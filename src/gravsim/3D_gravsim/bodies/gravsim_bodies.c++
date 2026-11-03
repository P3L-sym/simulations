#include "gravsim_bodies.h"
#include "../camera/gravsim_camera.h"
#include "../window/gravsim_window.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

// ══════════════════════════════════════════════════════════════════════════════
//  How the 3D rendering works (read this first)
// ══════════════════════════════════════════════════════════════════════════════
//
//  * Scale problem: the scene spans ~1e4 scene units but a small moon is ~1e-3.
//    32-bit floats can't hold both, so positions are subtracted from the camera
//    position in *double* on the CPU (Camera::toView) and only the small
//    camera-relative result goes to the GPU as float.
//
//  * No projection matrix: the vertex shaders project by hand
//        clip = (x * focal / aspect,  y * focal,  0,  -z)
//    and every fragment shader writes gl_FragDepth with a logarithmic depth
//    function, so near and far can be 1e-4 .. 1e5 without z-fighting.
//
//  * Planets are *impostors*: a camera-facing quad per body, ray-traced against
//    a perfect sphere in the fragment shader. Perfectly round at any zoom, and
//    no mesh to tessellate. Bodies too small to see are inflated to MIN_PIXELS.
//
//  * Rings are a flat quad in the planet's equatorial plane, ray-intersected
//    per pixel, with the planet's shadow cast onto them.

namespace {

constexpr float MIN_PIXELS = 2.0f;    // bodies never shrink below this radius on screen
constexpr float GLOW_SCALE = 1.8f;     // sun glow extent, in sun radii

// ── Shared GLSL ──────────────────────────────────────────────────────────────

static const char* GLSL_COMMON = R"GLSL(
// Logarithmic depth: constant relative precision from d = 1e-4 to d = 1e5.
float logDepth(float d) {
    const float K = 1.0e4;
    const float FAR = 1.0e5;
    return clamp(log2(1.0 + max(d, 0.0) * K) / log2(1.0 + FAR * K), 0.0, 1.0);
}
)GLSL";

static const char* GLSL_NOISE = R"GLSL(
float hash3(vec3 p) {
    p = fract(p * 0.3183099 + vec3(0.1, 0.2, 0.3));
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

float vnoise3(vec3 p) {
    vec3 i = floor(p), f = fract(p);
    vec3 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(mix(hash3(i + vec3(0,0,0)), hash3(i + vec3(1,0,0)), u.x),
                   mix(hash3(i + vec3(0,1,0)), hash3(i + vec3(1,1,0)), u.x), u.y),
               mix(mix(hash3(i + vec3(0,0,1)), hash3(i + vec3(1,0,1)), u.x),
                   mix(hash3(i + vec3(0,1,1)), hash3(i + vec3(1,1,1)), u.x), u.y), u.z);
}

float fbm3(vec3 p) {
    float v = 0.0, a = 0.5;
    for (int i = 0; i < 4; ++i) { v += a * vnoise3(p); p = p * 2.02 + 7.3; a *= 0.5; }
    return v;
}
)GLSL";

// Billboard that faces the body centre. The quad is sized to the tangent cone
// of the sphere so the whole silhouette fits even far off-axis.
static const char* BILLBOARD_VS = R"GLSL(
layout(location = 0) in vec2 aPos;

uniform vec3  uCenter;     // view space
uniform float uRadius;     // scene units
uniform float uInflate;    // 1 for the sphere, GLOW_SCALE for the glow
uniform float uFocal;
uniform float uAspect;

out vec3 vView;
out vec2 vLocal;

void main() {
    float len = length(uCenter);
    vec3 f = uCenter / len;
    vec3 helper = (abs(f.y) < 0.9) ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    vec3 u = normalize(cross(f, helper));
    vec3 v = cross(f, u);
    float s = uRadius * len * inversesqrt(max(len * len - uRadius * uRadius, 1e-30)) * uInflate * 1.02;

    vLocal = aPos;
    vView = uCenter + (u * aPos.x + v * aPos.y) * s;
    gl_Position = vec4(vView.x * uFocal / uAspect, vView.y * uFocal, 0.0, -vView.z);
}
)GLSL";

// ── Planet / star fragment shader ────────────────────────────────────────────

static const char* PLANET_FS = R"GLSL(
in vec3 vView;
in vec2 vLocal;
out vec4 fragColor;

uniform vec3  uCenter;
uniform float uRadius;
uniform vec3  uLightDir;   // view space, unit vector body -> sun
uniform vec3  uAxis;       // spin axis,           view space
uniform vec3  uEast;       // equatorial basis 1,  view space (body-fixed, world-derived)
uniform vec3  uNorth;      // equatorial basis 2 = axis x east
uniform float uRotAngle;   // spin phase, radians (positive = counter-clockwise seen from the north pole)
uniform int   uType;       // 0 star, 1 terrestrial, 2 gas giant, 3 ice giant, 4 icy moon
uniform vec3  uColor1;
uniform vec3  uColor2;
uniform vec3  uColor3;
uniform float uBoost;      // 0 = true size ... 1 = a sub-pixel body inflated to a dot: light it evenly so it stays visible

void main() {
    // Ray from the camera (origin) through this fragment vs. the sphere.
    vec3 d = normalize(vView);
    float b = dot(d, uCenter);
    vec3 perp = uCenter - d * b;
    float disc = uRadius * uRadius - dot(perp, perp);
    if (disc < 0.0) discard;

    vec3 hit = d * (b - sqrt(disc));
    vec3 n = (hit - uCenter) / uRadius;
    gl_FragDepth = logDepth(-hit.z);

    // Surface direction in the equatorial frame, then un-spun into body-fixed
    // coordinates so the surface pattern rotates with the planet.
    vec3 nb = vec3(dot(n, uEast), dot(n, uNorth), dot(n, uAxis));
    float cs = cos(uRotAngle), sn = sin(uRotAngle);
    vec3 p0 = vec3(cs * nb.x + sn * nb.y, -sn * nb.x + cs * nb.y, nb.z);

    float facing = clamp(dot(n, -d), 0.0, 1.0);

    // ── Star: self-lit ──
    if (uType == 0) {
        float grain = fbm3(p0 * 5.0);
        vec3 col = mix(uColor1, uColor2, grain);
        col += uColor3 * pow(1.0 - facing, 2.0) * 0.5;
        fragColor = vec4(col, 1.0);
        return;
    }

    float lat = asin(clamp(nb.z, -1.0, 1.0));
    vec3 base;
    if (uType == 2 || uType == 3) {
        float bandFreq = (uType == 2) ? 9.0 : 5.0;
        float band = sin(lat * bandFreq + fbm3(vec3(p0.xy * 1.5, p0.z * 6.0)) * 1.5);
        float m = smoothstep(-0.2, 0.2, band);
        base = mix(uColor1, uColor2, m);
        base = mix(base, uColor3, 0.15 * fbm3(p0 * 3.0));
    } else if (uType == 1) {
        float speck = fbm3(p0 * 3.0);
        base = mix(uColor1, uColor2, smoothstep(0.35, 0.65, speck));
        float polar = smoothstep(0.75, 0.95, abs(lat) / 1.5708);
        base = mix(base, uColor3, polar);
    } else {
        float speck = fbm3(p0 * 5.0);
        base = mix(uColor1, uColor2, smoothstep(0.4, 0.6, speck));
        float crack = abs(fbm3(p0 * 4.0 + 3.1) - 0.5);
        base = mix(uColor3, base, smoothstep(0.0, 0.03, crack));
    }

    float ndotl = dot(n, uLightDir);
    float diff = smoothstep(-0.15, 0.15, ndotl);
    float ambient = 0.06;
    float lighting = ambient + (1.0 - ambient) * diff;
    lighting = mix(lighting, 0.9, uBoost);

    float rim = pow(1.0 - facing, 3.0) * mix(0.15, 0.45, diff);
    vec3 col = base * lighting + vec3(0.6, 0.7, 1.0) * rim;
    fragColor = vec4(col, 1.0);
}
)GLSL";

// ── Sun glow ─────────────────────────────────────────────────────────────────

static const char* GLOW_FS = R"GLSL(
in vec3 vView;
in vec2 vLocal;
out vec4 fragColor;

uniform vec3  uCenter;
uniform vec3  uColor;
uniform float uCoreR;

void main() {
    // No cut-out for the core: the glow sits at the star's centre depth, so the
    // star's own (nearer) surface hides it. That avoids a seam at the limb.
    float r = length(vLocal);
    if (r > 1.0) discard;
    float t = clamp((r - uCoreR) / (1.0 - uCoreR), 0.0, 1.0);
    float glow = pow(1.0 - t, 3.0);
    gl_FragDepth = logDepth(-uCenter.z);
    fragColor = vec4(uColor, glow * 0.6);
}
)GLSL";

// ── Rings ────────────────────────────────────────────────────────────────────

static const char* RING_VS = R"GLSL(
layout(location = 0) in vec2 aPos;

uniform vec3  uCenter;
uniform vec3  uEast;
uniform vec3  uNorth;
uniform float uHalf;       // half-size of the quad, scene units
uniform float uFocal;
uniform float uAspect;

out vec3 vView;

void main() {
    vView = uCenter + (uEast * aPos.x + uNorth * aPos.y) * uHalf;
    gl_Position = vec4(vView.x * uFocal / uAspect, vView.y * uFocal, 0.0, -vView.z);
}
)GLSL";

static const char* RING_FS = R"GLSL(
in vec3 vView;
out vec4 fragColor;

uniform vec3  uCenter;
uniform float uRadius;     // planet radius, scene units (ring radii are multiples of this)
uniform vec3  uAxis;
uniform vec3  uEast;
uniform vec3  uNorth;
uniform vec3  uLightDir;
uniform float uInner;
uniform float uOuter;
uniform int   uStyle;      // 1 broad, 2 narrow, 3 faint
uniform vec3  uRingColor;
uniform float uRingAlpha;

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

void main() {
    // Intersect the pixel ray with the ring plane (through the planet centre).
    vec3 d = normalize(vView);
    float denom = dot(d, uAxis);
    if (abs(denom) < 1.0e-5) discard;
    float t = dot(uCenter, uAxis) / denom;
    if (t <= 0.0) discard;

    vec3 hit = d * t;
    vec3 loc = hit - uCenter;
    float ringDist = length(vec2(dot(loc, uEast), dot(loc, uNorth))) / uRadius;
    if (ringDist < uInner || ringDist > uOuter) discard;

    gl_FragDepth = logDepth(-hit.z);

    float edge = min(smoothstep(uInner, uInner + 0.05, ringDist),
                     1.0 - smoothstep(uOuter - 0.05, uOuter, ringDist));

    float alpha = uRingAlpha;
    if (uStyle == 1) {
        float band = sin(ringDist * 30.0);
        float gap = smoothstep(0.85, 0.95, abs(band));
        alpha *= mix(1.0, 0.15, gap);
        alpha *= 0.9 + 0.2 * hash(vec2(floor(ringDist * 50.0), 0.0));
    } else if (uStyle == 2) {
        float p = fract(ringDist * 8.0);
        alpha *= smoothstep(0.0, 0.08, p) * smoothstep(0.30, 0.22, p);
    } else {
        alpha *= 0.5 + 0.5 * hash(vec2(floor(ringDist * 20.0), 0.0));
    }

    // Planet's shadow: does the ray from this ring point toward the sun hit the planet?
    vec3 oc = hit - uCenter;
    float lb = dot(oc, uLightDir);
    float lc = dot(oc, oc) - uRadius * uRadius;
    float shadow = (lb < 0.0 && lb * lb - lc > 0.0) ? 1.0 : 0.0;

    // Viewing the unlit face of the ring dims it.
    float litSide = dot(uAxis, uLightDir) * dot(uAxis, -d);
    float side = (litSide >= 0.0) ? 1.0 : 0.45;

    vec3 col = uRingColor * side * mix(1.0, 0.12, shadow);
    fragColor = vec4(col, alpha * edge);
}
)GLSL";

// ── Trails ───────────────────────────────────────────────────────────────────

static const char* TRAIL_VS = R"GLSL(
layout(location = 0) in dvec3 aWorldPos;      // metres, relative to uOrigin

uniform dvec3 uOrigin;                        // metres: the parent's current position (0 for planets)
uniform int uHead;
uniform int uCount;
uniform int uCapacity;
uniform double uScale;
uniform dvec3 uEye;                           // scene units
uniform dvec3 uRight;
uniform dvec3 uUp;
uniform dvec3 uForward;
uniform float uFocal;
uniform float uAspect;

out float vAlpha;
out float vDepth;

void main() {
    dvec3 rel = (aWorldPos + uOrigin) * uScale - uEye;    // camera-relative, still double
    vec3 v = vec3(float(dot(rel, uRight)), float(dot(rel, uUp)), float(-dot(rel, uForward)));

    gl_Position = vec4(v.x * uFocal / uAspect, v.y * uFocal, 0.0, -v.z);
    vDepth = -v.z;

    int age = uHead - 1 - gl_VertexID;
    if (age < 0) age += uCapacity;
    float denom = float(max(uCount - 1, 1));
    float t = clamp(1.0 - float(age) / denom, 0.0, 1.0);
    vAlpha = t * t * 0.75;
}
)GLSL";

static const char* TRAIL_FS = R"GLSL(
in float vAlpha;
in float vDepth;
out vec4 fragColor;
uniform vec3 uColor;

void main() {
    gl_FragDepth = logDepth(vDepth);
    fragColor = vec4(uColor, vAlpha);
}
)GLSL";

// ── Starfield ────────────────────────────────────────────────────────────────

static const char* STAR_VS = R"GLSL(
layout(location = 0) in vec4 aStar;           // xyz = unit direction, w = brightness

uniform vec3  uRight;
uniform vec3  uUp;
uniform vec3  uForward;
uniform float uFocal;
uniform float uAspect;

out float vBright;

void main() {
    vec3 v = vec3(dot(aStar.xyz, uRight), dot(aStar.xyz, uUp), -dot(aStar.xyz, uForward));
    gl_Position = vec4(v.x * uFocal / uAspect, v.y * uFocal, 0.0, -v.z);
    gl_PointSize = 1.0 + 1.6 * aStar.w * aStar.w;
    vBright = 0.25 + 0.75 * aStar.w;
}
)GLSL";

static const char* STAR_FS = R"GLSL(
in float vBright;
out vec4 fragColor;
void main() {
    fragColor = vec4(vec3(0.85, 0.9, 1.0) * vBright, vBright);
}
)GLSL";

// ── GL helpers ───────────────────────────────────────────────────────────────

GLuint compileShader(GLenum stage, const std::string& src) {
    GLuint sh = glCreateShader(stage);
    const char* p = src.c_str();
    glShaderSource(sh, 1, &p, nullptr);
    glCompileShader(sh);
    GLint ok = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(sh, sizeof(log), nullptr, log);
        std::fprintf(stderr, "[Body] shader compile error:\n%s\n", log);
    }
    return sh;
}

// Every shader gets the version line + the shared depth helper; fragment
// shaders additionally get the noise helpers.
GLuint linkProgram(const char* vsBody, const char* fsBody, bool noise = false) {
    std::string head = std::string("#version 430\n") + GLSL_COMMON;
    std::string vsSrc = head + vsBody;
    std::string fsSrc = head + (noise ? GLSL_NOISE : "") + fsBody;

    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint linked = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[2048];
        glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
        std::fprintf(stderr, "[Body] shader link error:\n%s\n", log);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

GLuint makeQuadVAO(GLuint& vboOut) {
    GLuint vao;
    float quad[8] = { -1,-1,  1,-1,  -1,1,  1,1 };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboOut);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vboOut);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return vao;
}

inline void setVec3(GLint loc, const Vec3& v) { glUniform3f(loc, (float)v.x, (float)v.y, (float)v.z); }

// ── Cached GL state ──────────────────────────────────────────────────────────

struct PlanetGL {
    GLuint prog = 0, vao = 0, vbo = 0;
    GLint center, radius, inflate, focal, aspect, lightDir, axis, east, north, rot, type, c1, c2, c3, boost;
    bool ready = false;
};

PlanetGL& planetGL() {
    static PlanetGL s;
    if (s.ready) return s;
    s.prog = linkProgram(BILLBOARD_VS, PLANET_FS, true);
    s.vao  = makeQuadVAO(s.vbo);
    auto L = [&](const char* n) { return glGetUniformLocation(s.prog, n); };
    s.center = L("uCenter"); s.radius = L("uRadius"); s.inflate = L("uInflate");
    s.focal = L("uFocal");   s.aspect = L("uAspect"); s.lightDir = L("uLightDir");
    s.axis = L("uAxis");     s.east = L("uEast");     s.north = L("uNorth");
    s.rot = L("uRotAngle");  s.type = L("uType");
    s.c1 = L("uColor1");     s.c2 = L("uColor2");     s.c3 = L("uColor3");
    s.boost = L("uBoost");
    s.ready = true;
    return s;
}

struct GlowGL {
    GLuint prog = 0;
    GLint center, radius, inflate, focal, aspect, color, coreR;
    bool ready = false;
};

GlowGL& glowGL() {
    static GlowGL s;
    if (s.ready) return s;
    s.prog = linkProgram(BILLBOARD_VS, GLOW_FS);
    auto L = [&](const char* n) { return glGetUniformLocation(s.prog, n); };
    s.center = L("uCenter"); s.radius = L("uRadius"); s.inflate = L("uInflate");
    s.focal = L("uFocal");   s.aspect = L("uAspect"); s.color = L("uColor"); s.coreR = L("uCoreR");
    s.ready = true;
    return s;
}

struct RingGL {
    GLuint prog = 0, vao = 0, vbo = 0;
    GLint center, radius, half, focal, aspect, axis, east, north, lightDir;
    GLint inner, outer, style, color, alpha;
    bool ready = false;
};

RingGL& ringGL() {
    static RingGL s;
    if (s.ready) return s;
    s.prog = linkProgram(RING_VS, RING_FS);
    s.vao  = makeQuadVAO(s.vbo);
    auto L = [&](const char* n) { return glGetUniformLocation(s.prog, n); };
    s.center = L("uCenter"); s.radius = L("uRadius"); s.half = L("uHalf");
    s.focal = L("uFocal");   s.aspect = L("uAspect"); s.axis = L("uAxis");
    s.east = L("uEast");     s.north = L("uNorth");   s.lightDir = L("uLightDir");
    s.inner = L("uInner");   s.outer = L("uOuter");   s.style = L("uStyle");
    s.color = L("uRingColor"); s.alpha = L("uRingAlpha");
    s.ready = true;
    return s;
}

struct TrailGL {
    GLuint prog = 0;
    GLint origin, head, count, capacity, scale, eye, right, up, forward, focal, aspect, color;
    bool ready = false;
};

TrailGL& trailGL() {
    static TrailGL s;
    if (s.ready) return s;
    s.prog = linkProgram(TRAIL_VS, TRAIL_FS);
    auto L = [&](const char* n) { return glGetUniformLocation(s.prog, n); };
    s.origin = L("uOrigin");
    s.head = L("uHead");   s.count = L("uCount"); s.capacity = L("uCapacity");
    s.scale = L("uScale"); s.eye = L("uEye");     s.right = L("uRight");
    s.up = L("uUp");       s.forward = L("uForward");
    s.focal = L("uFocal"); s.aspect = L("uAspect"); s.color = L("uColor");
    s.ready = true;
    return s;
}

struct StarGL {
    GLuint prog = 0, vao = 0, vbo = 0;
    GLint right, up, forward, focal, aspect;
    int count = 0;
    bool ready = false;
};

StarGL& starGL() {
    static StarGL s;
    if (s.ready) return s;
    s.prog = linkProgram(STAR_VS, STAR_FS);

    // Deterministic uniform points on the unit sphere.
    constexpr int N = 5000;
    std::vector<float> data;
    data.reserve(N * 4);
    unsigned int state = 12345u;
    auto rnd = [&]() {
        state = state * 1664525u + 1013904223u;
        return (state >> 8) * (1.0f / 16777216.0f);
    };
    for (int i = 0; i < N; ++i) {
        float z = 2.0f * rnd() - 1.0f;
        float a = 6.2831853f * rnd();
        float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
        float b = std::pow(rnd(), 3.0f);       // mostly dim, a few bright
        data.push_back(r * std::cos(a));
        data.push_back(r * std::sin(a));
        data.push_back(z);
        data.push_back(b);
    }
    s.count = N;

    glGenVertexArrays(1, &s.vao);
    glGenBuffers(1, &s.vbo);
    glBindVertexArray(s.vao);
    glBindBuffer(GL_ARRAY_BUFFER, s.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    auto L = [&](const char* n) { return glGetUniformLocation(s.prog, n); };
    s.right = L("uRight"); s.up = L("uUp"); s.forward = L("uForward");
    s.focal = L("uFocal"); s.aspect = L("uAspect");
    s.ready = true;
    return s;
}

// ── Per-body view data ───────────────────────────────────────────────────────

struct BodyView {
    Vec3   center;      // view space
    double dist;        // |center|
    float  radius;      // drawn radius (>= the real radius)
    float  realRadius;
    Vec3   light;       // unit vector toward the sun, view space
    Vec3   axis, east, north;   // view space
};

// Returns false if the body is off-screen-behind or the camera is inside it.
bool computeView(const Body& b, const Camera& cam, BodyView& out) {
    Vec3 sp = b.pos * scale;
    out.center = cam.toView(sp);
    out.dist = out.center.length();
    if (out.dist <= 0.0) return false;

    double real = (double)b.drawRadius * scale;
    double minDrawn = MIN_PIXELS * out.dist / cam.focalPx;
    double drawn = std::max(real, minDrawn);

    if (out.center.z > drawn) return false;       // wholly behind the camera
    if (out.dist <= real * 1.001) return false;   // camera is inside the body

    out.radius = (float)drawn;
    out.realRadius = (float)real;

    out.light = cam.dirToView((cam.sunWorld - sp).normalized());

    // Spin frame, defined in world space so the surface pattern is body-fixed
    // (not camera-fixed), then rotated into view space.
    Vec3 axisW = b.spinAxis();
    Vec3 refW = (std::abs(axisW.y) < 0.99) ? Vec3(0.0, 1.0, 0.0) : Vec3(1.0, 0.0, 0.0);
    Vec3 eastW = axisW.cross(refW).normalized();
    Vec3 northW = axisW.cross(eastW);               // east x north = axis (right-handed)
    out.axis  = cam.dirToView(axisW);
    out.east  = cam.dirToView(eastW);
    out.north = cam.dirToView(northW);
    return true;
}

} // namespace

// ══════════════════════════════════════════════════════════════════════════════
//  Body
// ══════════════════════════════════════════════════════════════════════════════

Body::Body(Vec3 pos, Vec3 vel, double mass,
           float drawRadius, float r, float g, float b)
    : pos(pos), vel(vel), acc{0, 0, 0},
      mass(mass), drawRadius(drawRadius),
      r(r), g(g), b(b) {}

void Body::configureRotation(double periodSeconds, double tiltDeg, PlanetType t,
                              float c2r_, float c2g_, float c2b_,
                              float c3r_, float c3g_, float c3b_) {
    rotationPeriod = periodSeconds;
    axialTilt = tiltDeg * 3.14159265358979323846 / 180.0;
    type = t;
    c2r = c2r_; c2g = c2g_; c2b = c2b_;
    c3r = c3r_; c3g = c3g_; c3b = c3b_;
}

void Body::configureRings(float innerMult, float outerMult, RingStyle style,
                           float rr, float rg, float rb, float alpha) {
    hasRings = true;
    ringInner = innerMult;
    ringOuter = outerMult;
    ringStyle = style;
    ringR = rr; ringG = rg; ringB = rb;
    ringAlpha = alpha;
}

void Body::updateRotation(double dtRealSeconds) {
    if (rotationPeriod != 0.0)
        rotationAngle += (2.0 * 3.14159265358979323846 / rotationPeriod) * dtRealSeconds;
}

void Body::draw(const Camera& cam) const {
    BodyView v;
    if (!computeView(*this, cam, v)) return;
    PlanetGL& s = planetGL();

    float col2r = c2r >= 0.f ? c2r : r, col2g = c2g >= 0.f ? c2g : g, col2b = c2b >= 0.f ? c2b : b;
    float col3r = c3r >= 0.f ? c3r : r, col3g = c3g >= 0.f ? c3g : g, col3b = c3b >= 0.f ? c3b : b;

    glUseProgram(s.prog);
    setVec3(s.center, v.center);
    glUniform1f(s.radius, v.radius);
    glUniform1f(s.inflate, 1.0f);
    glUniform1f(s.focal, (float)cam.focal);
    glUniform1f(s.aspect, (float)cam.aspect);
    setVec3(s.lightDir, v.light);
    setVec3(s.axis, v.axis);
    setVec3(s.east, v.east);
    setVec3(s.north, v.north);
    glUniform1f(s.rot, (float)std::fmod(rotationAngle, 2.0 * 3.14159265358979323846));
    glUniform1i(s.type, (int)type);
    glUniform3f(s.c1, r, g, b);
    glUniform3f(s.c2, col2r, col2g, col2b);
    glUniform3f(s.c3, col3r, col3g, col3b);
    glUniform1f(s.boost, std::clamp(1.0f - v.realRadius / v.radius, 0.0f, 1.0f));

    glBindVertexArray(s.vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Body::drawGlow(const Camera& cam) const {
    if (type != PlanetType::Star) return;
    BodyView v;
    if (!computeView(*this, cam, v)) return;
    GlowGL& s = glowGL();

    glUseProgram(s.prog);
    setVec3(s.center, v.center);
    glUniform1f(s.radius, v.radius);
    glUniform1f(s.inflate, GLOW_SCALE);
    glUniform1f(s.focal, (float)cam.focal);
    glUniform1f(s.aspect, (float)cam.aspect);
    glUniform3f(s.color, r, g, b);
    glUniform1f(s.coreR, 1.0f / GLOW_SCALE);

    glBindVertexArray(planetGL().vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Body::drawRings(const Camera& cam) const {
    if (!hasRings) return;
    BodyView v;
    if (!computeView(*this, cam, v)) return;
    RingGL& s = ringGL();

    glUseProgram(s.prog);
    setVec3(s.center, v.center);
    glUniform1f(s.radius, v.radius);
    glUniform1f(s.half, v.radius * ringOuter);
    glUniform1f(s.focal, (float)cam.focal);
    glUniform1f(s.aspect, (float)cam.aspect);
    setVec3(s.axis, v.axis);
    setVec3(s.east, v.east);
    setVec3(s.north, v.north);
    setVec3(s.lightDir, v.light);
    glUniform1f(s.inner, ringInner);
    glUniform1f(s.outer, ringOuter);
    glUniform1i(s.style, (int)ringStyle);
    glUniform3f(s.color, ringR, ringG, ringB);
    glUniform1f(s.alpha, ringAlpha);

    glBindVertexArray(s.vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Body::drawTrail(const Camera& cam, Vec3 origin) const {
    trail.draw(cam, r, g, b, origin);
}

// ══════════════════════════════════════════════════════════════════════════════
//  Trail
// ══════════════════════════════════════════════════════════════════════════════

Trail::Trail(const Trail& o)
    : points(o.points), capacity(o.capacity), head(o.head), count(o.count) {}

Trail& Trail::operator=(const Trail& o) {
    if (this == &o) return *this;
    points = o.points; capacity = o.capacity; head = o.head; count = o.count;
    vao = 0; vbo = 0; glReady = false;
    return *this;
}

Trail::~Trail() {
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

void Trail::push(Vec3 p) {
    if (capacity == 0) {
        capacity = trail_length;
        points.assign((size_t)capacity * 3, 0.0);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(capacity * 3 * sizeof(double)),
                     points.data(), GL_DYNAMIC_DRAW);
        glVertexAttribLPointer(0, 3, GL_DOUBLE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glReady = true;
    }

    points[(size_t)head * 3 + 0] = p.x;
    points[(size_t)head * 3 + 1] = p.y;
    points[(size_t)head * 3 + 2] = p.z;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)((size_t)head * 3 * sizeof(double)),
                    (GLsizeiptr)(3 * sizeof(double)), &points[(size_t)head * 3]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    head = (head + 1) % capacity;
    if (count < capacity) count++;
}

void Trail::draw(const Camera& cam, float r, float g, float b, Vec3 origin) const {
    if (!glReady || count < 2) return;
    TrailGL& s = trailGL();

    glUseProgram(s.prog);
    glUniform3d(s.origin, origin.x, origin.y, origin.z);
    glUniform1i(s.head, head);
    glUniform1i(s.count, count);
    glUniform1i(s.capacity, capacity);
    glUniform1d(s.scale, scale);
    glUniform3d(s.eye, cam.eye.x, cam.eye.y, cam.eye.z);
    glUniform3d(s.right, cam.right.x, cam.right.y, cam.right.z);
    glUniform3d(s.up, cam.up.x, cam.up.y, cam.up.z);
    glUniform3d(s.forward, cam.forward.x, cam.forward.y, cam.forward.z);
    glUniform1f(s.focal, (float)cam.focal);
    glUniform1f(s.aspect, (float)cam.aspect);
    glUniform3f(s.color, r, g, b);

    glBindVertexArray(vao);
    if (count < capacity) {
        glDrawArrays(GL_LINE_STRIP, 0, count);
    } else if (head == 0) {
        glDrawArrays(GL_LINE_STRIP, 0, capacity);
    } else {
        glDrawArrays(GL_LINE_STRIP, head, capacity - head);
        glDrawArrays(GL_LINE_STRIP, 0, head);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

// ══════════════════════════════════════════════════════════════════════════════
//  Starfield
// ══════════════════════════════════════════════════════════════════════════════

void drawStarfield(const Camera& cam) {
    StarGL& s = starGL();

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glUseProgram(s.prog);
    setVec3(s.right, cam.right);
    setVec3(s.up, cam.up);
    setVec3(s.forward, cam.forward);
    glUniform1f(s.focal, (float)cam.focal);
    glUniform1f(s.aspect, (float)cam.aspect);

    glBindVertexArray(s.vao);
    glDrawArrays(GL_POINTS, 0, s.count);
    glBindVertexArray(0);
    glUseProgram(0);

    glDisable(GL_PROGRAM_POINT_SIZE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}
