#include "gravsim_compute.h"
#include "../window/gravsim_window.h"
#include <cstdio>
#include <cstring>

// ── Shader sources ────────────────────────────────────────────────────────────
// The Body struct here MUST match BodyGPU in the header.

static const char* KICK_POS_SRC = R"GLSL(
#version 430
layout(local_size_x = 64) in;

struct Body {
    double px, py, pz;
    double vx, vy, vz;
    double ax, ay, az;
    double mass;
};
layout(std430, binding = 0) buffer BodyBuf { Body bodies[]; };

uniform int n;
uniform double c_dt;

void main() {
    uint i = gl_GlobalInvocationID.x;
    if (i >= uint(n)) return;
    bodies[i].px += bodies[i].vx * c_dt;
    bodies[i].py += bodies[i].vy * c_dt;
    bodies[i].pz += bodies[i].vz * c_dt;
}
)GLSL";

// O(N^2) all-pairs force kernel, tiled through shared memory so it works for
// any N (the 2D version only worked up to 64 bodies).
//
// Every thread must reach every barrier(), so out-of-range threads do NOT
// return early; they just skip the accumulation and the final write.
static const char* KICK_VEL_SRC = R"GLSL(
#version 430
layout(local_size_x = 64) in;

struct Body {
    double px, py, pz;
    double vx, vy, vz;
    double ax, ay, az;
    double mass;
};
layout(std430, binding = 0) buffer BodyBuf { Body bodies[]; };

uniform int n;
uniform double d_dt;
uniform double G_val;
uniform double soft2;

shared double s_x[64];
shared double s_y[64];
shared double s_z[64];
shared double s_m[64];

void main() {
    uint i  = gl_GlobalInvocationID.x;
    uint li = gl_LocalInvocationID.x;
    bool valid = (i < uint(n));

    dvec3 pi = valid ? dvec3(bodies[i].px, bodies[i].py, bodies[i].pz) : dvec3(0.0);
    dvec3 ai = dvec3(0.0);

    uint tiles = (uint(n) + 63u) / 64u;
    for (uint t = 0u; t < tiles; ++t) {
        uint j = t * 64u + li;
        if (j < uint(n)) {
            s_x[li] = bodies[j].px;
            s_y[li] = bodies[j].py;
            s_z[li] = bodies[j].pz;
            s_m[li] = bodies[j].mass;
        } else {
            s_x[li] = 0.0; s_y[li] = 0.0; s_z[li] = 0.0; s_m[li] = 0.0;
        }
        barrier();

        if (valid) {
            for (uint k = 0u; k < 64u; ++k) {
                uint jj = t * 64u + k;
                if (jj >= uint(n) || jj == i) continue;
                dvec3 delta = dvec3(s_x[k], s_y[k], s_z[k]) - pi;
                double r2   = dot(delta, delta) + soft2;
                double inv  = inversesqrt(r2);
                ai += delta * (G_val * s_m[k] * inv * inv * inv);
            }
        }
        barrier();
    }

    if (valid) {
        bodies[i].vx += ai.x * d_dt;
        bodies[i].vy += ai.y * d_dt;
        bodies[i].vz += ai.z * d_dt;
        bodies[i].ax = ai.x;
        bodies[i].ay = ai.y;
        bodies[i].az = ai.z;
    }
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

    m_locPosN    = glGetUniformLocation(m_kickPosPrg, "n");
    m_locPosDt   = glGetUniformLocation(m_kickPosPrg, "c_dt");
    m_locVelN    = glGetUniformLocation(m_kickVelPrg, "n");
    m_locVelDt   = glGetUniformLocation(m_kickVelPrg, "d_dt");
    m_locVelG    = glGetUniformLocation(m_kickVelPrg, "G_val");
    m_locVelSoft = glGetUniformLocation(m_kickVelPrg, "soft2");
}

// ── Data transfer ─────────────────────────────────────────────────────────────

void GravCompute::upload(const std::vector<Body>& bodies) {
    std::vector<BodyGPU> buf(m_n);
    for (int i = 0; i < m_n; ++i) {
        buf[i].pos[0] = bodies[i].pos.x; buf[i].pos[1] = bodies[i].pos.y; buf[i].pos[2] = bodies[i].pos.z;
        buf[i].vel[0] = bodies[i].vel.x; buf[i].vel[1] = bodies[i].vel.y; buf[i].vel[2] = bodies[i].vel.z;
        buf[i].acc[0] = bodies[i].acc.x; buf[i].acc[1] = bodies[i].acc.y; buf[i].acc[2] = bodies[i].acc.z;
        buf[i].mass   = bodies[i].mass;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                    m_n * (GLsizeiptr)sizeof(BodyGPU), buf.data());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GravCompute::download(std::vector<Body>& bodies) const {
    std::vector<BodyGPU> buf(m_n);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                       m_n * (GLsizeiptr)sizeof(BodyGPU), buf.data());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    for (int i = 0; i < m_n; ++i) {
        bodies[i].pos = Vec3(buf[i].pos[0], buf[i].pos[1], buf[i].pos[2]);
        bodies[i].vel = Vec3(buf[i].vel[0], buf[i].vel[1], buf[i].vel[2]);
        bodies[i].acc = Vec3(buf[i].acc[0], buf[i].acc[1], buf[i].acc[2]);
    }
}

// ── Per-step dispatch ─────────────────────────────────────────────────────────

void GravCompute::kickPos(double c_dt) {
    glUseProgram(m_kickPosPrg);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glUniform1i(m_locPosN, m_n);
    glUniform1d(m_locPosDt, c_dt);
    glDispatchCompute((m_n + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GravCompute::kickVel(double d_dt) {
    glUseProgram(m_kickVelPrg);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glUniform1i(m_locVelN, m_n);
    glUniform1d(m_locVelDt, d_dt);
    glUniform1d(m_locVelG, G);
    glUniform1d(m_locVelSoft, softening * softening);
    glDispatchCompute((m_n + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GravCompute::integrateStep(double dt) {
    // Yoshida 4th-order symplectic coefficients, same as the CPU version.
    static const double cbrt2 = std::cbrt(2.0);
    static const double w1 = 1.0 / (2.0 - cbrt2);
    static const double w0 = -cbrt2 * w1;
    static const double c1 = w1 * 0.5;
    static const double c2 = (w0 + w1) * 0.5;
    static const double d1 = w1;
    static const double d2 = w0;

    kickPos(c1 * dt);
    kickVel(d1 * dt);
    kickPos(c2 * dt);
    kickVel(d2 * dt);
    kickPos(c2 * dt);
    kickVel(d1 * dt);
    kickPos(c1 * dt);
}
