#include "gravsim_compute.h"
#include "../window/gravsim_window.h"
#include <cstdio>
#include <cstring>

// ── Shader sources ────────────────────────────────────────────────────────────
//
// Both shaders share the same SSBO layout. The Body struct here MUST match
// BodyGPU in the header (std430 packs dvec2 at 16-byte alignment).

static const char* KICK_POS_SRC = R"GLSL(
#version 430
layout(local_size_x = 64) in;

struct Body { dvec2 pos; dvec2 vel; dvec2 acc; double mass; double _pad; };
layout(std430, binding = 0) buffer BodyBuf { Body bodies[]; };

uniform int n;
uniform double c_dt;

void main() {
    uint i = gl_GlobalInvocationID.x;
    if (i >= uint(n)) return;
    bodies[i].pos += bodies[i].vel * c_dt;
}
)GLSL";

// kickVel: O(N²) all-pairs force kernel.
//
// Shared-memory optimisation: because N ≤ 64 (our workgroup size), every
// thread cooperatively loads one body into shared memory before the inner
// loop.  This cuts global-memory reads from N² to N, which matters once
// bodies grow beyond L1 cache.
//
// If you add more than 64 bodies, either raise local_size_x (up to the
// hardware limit, typically 1024) or switch to a tiled loop.
static const char* KICK_VEL_SRC = R"GLSL(
#version 430
layout(local_size_x = 64) in;

struct Body { dvec2 pos; dvec2 vel; dvec2 acc; double mass; double _pad; };
layout(std430, binding = 0) buffer BodyBuf { Body bodies[]; };

uniform int n;
uniform double d_dt;
uniform double G_val;
uniform double soft2;   

shared dvec2  s_pos[64];
shared double s_mass[64];

void main() {
    uint i  = gl_GlobalInvocationID.x;
    uint li = gl_LocalInvocationID.x;

    // Cooperative load — each thread loads one body into shared memory.
    if (li < uint(n)) {
        s_pos[li]  = bodies[li].pos;
        s_mass[li] = bodies[li].mass;
    } else {
        s_pos[li]  = dvec2(0.0);
        s_mass[li] = 0.0;
    }
    barrier();  // wait for all threads to finish loading

    if (i >= uint(n)) return;

    dvec2  ai = dvec2(0.0);
    dvec2  pi = s_pos[i];

    for (int j = 0; j < n; ++j) {
        if (j == int(i)) continue;
        dvec2  delta = s_pos[j] - pi;
        double r2    = dot(delta, delta) + soft2;
        double inv   = inversesqrt(r2);          // fast GPU rsqrt
        ai += delta * (G_val * s_mass[j] * inv * inv * inv);
    }

    bodies[i].vel += ai * d_dt;
    bodies[i].acc  = ai;
}
)GLSL";

// ── Shader compilation helper ─────────────────────────────────────────────────

GLuint GravCompute::makeComputeProgram(const char* src) {
    GLuint sh = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);

    GLint ok = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(sh, sizeof(log), nullptr, log);
        std::fprintf(stderr, "[GravCompute] compile error:\n%s\n", log);
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, sh);
    glLinkProgram(prog);

    GLint linked = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[2048];
        glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
        std::fprintf(stderr, "[GravCompute] link error:\n%s\n", log);
    }

    glDeleteShader(sh);
    return prog;
}

// ── Lifecycle ─────────────────────────────────────────────────────────────────

GravCompute::~GravCompute() {
    if (m_ssbo)       glDeleteBuffers(1, &m_ssbo);
    if (m_kickPosPrg) glDeleteProgram(m_kickPosPrg);
    if (m_kickVelPrg) glDeleteProgram(m_kickVelPrg);
}

void GravCompute::init(int numBodies) {
    m_n = numBodies;

    glGenBuffers(1, &m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 m_n * (GLsizeiptr)sizeof(BodyGPU),
                 nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    m_kickPosPrg = makeComputeProgram(KICK_POS_SRC);
    m_kickVelPrg = makeComputeProgram(KICK_VEL_SRC);
}

// ── Data transfer ─────────────────────────────────────────────────────────────

void GravCompute::upload(const std::vector<Body>& bodies) {
    std::vector<BodyGPU> buf(m_n);
    for (int i = 0; i < m_n; ++i) {
        buf[i].pos[0]  = bodies[i].pos.x;  buf[i].pos[1]  = bodies[i].pos.y;
        buf[i].vel[0]  = bodies[i].vel.x;  buf[i].vel[1]  = bodies[i].vel.y;
        buf[i].acc[0]  = bodies[i].acc.x;  buf[i].acc[1]  = bodies[i].acc.y;
        buf[i].mass    = bodies[i].mass;
        buf[i]._pad    = 0.0;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                    m_n * (GLsizeiptr)sizeof(BodyGPU), buf.data());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GravCompute::download(std::vector<Body>& bodies) const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    const auto* buf = static_cast<const BodyGPU*>(
        glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));

    for (int i = 0; i < m_n; ++i) {
        bodies[i].pos = Vec2(buf[i].pos[0], buf[i].pos[1]);  // ← was { ... }
        bodies[i].vel = Vec2(buf[i].vel[0], buf[i].vel[1]);
        bodies[i].acc = Vec2(buf[i].acc[0], buf[i].acc[1]);
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

// ── Per-step dispatch ─────────────────────────────────────────────────────────

void GravCompute::kickPos(double c_dt) {
    glUseProgram(m_kickPosPrg);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glUniform1i(glGetUniformLocation(m_kickPosPrg, "n"),    m_n);
    glUniform1d(glGetUniformLocation(m_kickPosPrg, "c_dt"), c_dt);
    glDispatchCompute((m_n + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GravCompute::kickVel(double d_dt) {
    glUseProgram(m_kickVelPrg);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glUniform1i(glGetUniformLocation(m_kickVelPrg, "n"),     m_n);
    glUniform1d(glGetUniformLocation(m_kickVelPrg, "d_dt"),  d_dt);
    glUniform1d(glGetUniformLocation(m_kickVelPrg, "G_val"), G);
    glUniform1d(glGetUniformLocation(m_kickVelPrg, "soft2"), softening * softening);
    glDispatchCompute((m_n + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GravCompute::integrateStep(double dt) {
    // Yoshida 4th-order symplectic coefficients — same as your CPU version
    static const double cbrt2 = std::cbrt(2.0);
    static const double w1 = 1.0 / (2.0 - cbrt2);
    static const double w0 = -cbrt2 * w1;
    static const double c1 = w1 * 0.5;
    static const double c2 = (w0 + w1) * 0.5;
    static const double d1 = w1;
    static const double d2 = w0;

    // All 7 sub-steps run on GPU — zero CPU-GPU round trips per step
    kickPos(c1 * dt);
    kickVel(d1 * dt);
    kickPos(c2 * dt);
    kickVel(d2 * dt);
    kickPos(c2 * dt);
    kickVel(d1 * dt);
    kickPos(c1 * dt);
}