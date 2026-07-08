#include "gravsim_bodies.h"
#include "../camera/gravsim_camera.h"
#include "../window/gravsim_window.h"

#include <cmath>
#include <cstdio>

// ── Planet shader sources ─────────────────────────────────────────────────────

static const char* PLANET_VS = R"GLSL(
#version 430
layout(location = 0) in vec2 aPos;

uniform vec2 uCenterPx;
uniform float uRadiusPx;
uniform float uScreenW;
uniform float uScreenH;

out vec2 vLocal;

void main() {
    vLocal = aPos;
    vec2 screenPos = uCenterPx + aPos * uRadiusPx;
    float ndcX = (screenPos.x / uScreenW) * 2.0 - 1.0;
    float ndcY = 1.0 - (screenPos.y / uScreenH) * 2.0;
    gl_Position = vec4(ndcX, ndcY, 0.0, 1.0);
}
)GLSL";

static const char* PLANET_FS = R"GLSL(
#version 430
in vec2 vLocal;
out vec4 fragColor;

uniform vec2  uLightDir;   // normalized 2D world-space direction from body -> sun
uniform float uTiltRad;    // axial tilt, radians
uniform float uRotAngle;   // current spin phase, radians
uniform int   uType;       // 0 star, 1 terrestrial, 2 gas giant, 3 ice giant, 4 icy moon
uniform float uCoreR;      // local-space radius of the visible disc (1.0 except for the star's glow)
uniform vec3  uColor1;
uniform vec3  uColor2;
uniform vec3  uColor3;

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

float noise(vec2 p) {
    vec2 i = floor(p), f = fract(p);
    float a = hash(i),         b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0)), d = hash(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

float fbm(vec2 p) {
    float v = 0.0, a = 0.5;
    for (int i = 0; i < 4; ++i) { v += a * noise(p); p *= 2.02; a *= 0.5; }
    return v;
}

void main() {
    float r2 = dot(vLocal, vLocal);

    // ── Sun: self-lit, allowed to bleed past the core radius for a soft glow ──
    if (uType == 0) {
        float r = length(vLocal);
        if (r > 1.0) discard;
        if (r <= uCoreR) {
            vec2 pl = vLocal / uCoreR;
            float z = sqrt(max(1.0 - dot(pl, pl), 0.0));
            vec2 p = pl * 3.0 + vec2(cos(uRotAngle), sin(uRotAngle)) * 0.4;
            float grain = fbm(p * 2.5);
            vec3 col = mix(uColor1, uColor2, grain);
            col += uColor3 * pow(1.0 - z, 2.0) * 0.5; // limb brightening
            fragColor = vec4(col, 1.0);
        } else {
            float t = (r - uCoreR) / (1.0 - uCoreR);
            float glow = pow(1.0 - t, 3.0);
            fragColor = vec4(uColor1, glow * 0.6);
        }
        return;
    }

    // ── Planets & moons: shaded sphere ──────────────────────────────────────
    if (r2 > 1.0) discard;
    float z = sqrt(max(1.0 - r2, 0.0));
    vec3 n = normalize(vec3(vLocal, z));

    vec3 axis   = normalize(vec3(sin(uTiltRad), 0.0, cos(uTiltRad)));
    vec3 refv   = (abs(axis.y) < 0.99) ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    vec3 eastv  = normalize(cross(axis, refv));
    vec3 northv = cross(eastv, axis);

    float lat = asin(clamp(dot(n, axis), -1.0, 1.0));
    float lon = atan(dot(n, northv), dot(n, eastv)) + uRotAngle;

    vec3 base;
    if (uType == 2 || uType == 3) {
        // Gas / ice giant: latitude banding + turbulence
        float bandFreq = (uType == 2) ? 9.0 : 5.0;
        float band = sin(lat * bandFreq + fbm(vec2(lon * 1.5, lat * 6.0)) * 1.5);
        float m = smoothstep(-0.2, 0.2, band);
        base = mix(uColor1, uColor2, m);
        base = mix(base, uColor3, 0.15 * fbm(vec2(lon * 3.0 + uRotAngle * 0.2, lat * 3.0)));
    } else if (uType == 1) {
        // Terrestrial: continents/craters + polar caps
        float speck = fbm(vec2(lon * 4.0, lat * 4.0));
        base = mix(uColor1, uColor2, smoothstep(0.35, 0.65, speck));
        float polar = smoothstep(0.75, 0.95, abs(lat) / 1.5708);
        base = mix(base, uColor3, polar);
    } else {
        // Icy moon: flat-ish with crater speckle + faint crack lines
        float speck = fbm(vec2(lon * 6.0, lat * 6.0));
        base = mix(uColor1, uColor2, smoothstep(0.4, 0.6, speck));
        float crack = abs(fract(lon * 2.0 + lat * 3.0) - 0.5);
        base = mix(uColor3, base, smoothstep(0.0, 0.04, crack));
    }

    vec3 lightDir3 = normalize(vec3(uLightDir, 0.0));
    float ndotl = dot(n, lightDir3);
    float diff = smoothstep(-0.15, 0.15, ndotl); // soft terminator
    float ambient = 0.06;
    float lighting = ambient + (1.0 - ambient) * diff;

    float rim = pow(1.0 - z, 3.0) * mix(0.15, 0.45, diff);
    vec3 col = base * lighting + vec3(0.6, 0.7, 1.0) * rim;

    float edgeAA = 1.0 - smoothstep(0.96, 1.0, sqrt(r2));
    fragColor = vec4(col, edgeAA);
}
)GLSL";

// ── Ring shader sources ───────────────────────────────────────────────────────

static const char* RING_VS = PLANET_VS;

static const char* RING_FS = R"GLSL(
#version 430
in vec2 vLocal;
out vec4 fragColor;

uniform float uTiltRad;
uniform float uInner;      // inner ring radius, planet-radius units
uniform float uOuter;      // outer ring radius, planet-radius units
uniform int   uStyle;      // 1 broad, 2 narrow, 3 faint
uniform int   uBackPass;   // 1 = far half only, 0 = near half only
uniform vec2  uLightDir;
uniform vec3  uRingColor;
uniform float uRingAlpha;

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

void main() {
    float s = cos(uTiltRad);
    float sSafe = (abs(s) < 0.06) ? ((s < 0.0) ? -0.06 : 0.06) : s;

    // Un-squish the projected point back into the ring plane.
    vec2 w = vec2(-vLocal.x / sSafe, vLocal.y);
    float ringDist = length(w);
    if (ringDist < uInner || ringDist > uOuter) discard;

    float zVal = w.x * sin(uTiltRad);
    if (uBackPass == 1 && zVal >= 0.0) discard;
    if (uBackPass == 0 && zVal <  0.0) discard;

    float edge = min(smoothstep(uInner, uInner + 0.05, ringDist),
                      1.0 - smoothstep(uOuter - 0.05, uOuter, ringDist));

    float alpha = uRingAlpha;
    vec3 col = uRingColor;

    if (uStyle == 1) {
        // Broad, bright ring with faint gap bands (Saturn-like)
        float band = sin(ringDist * 30.0);
        float gap = smoothstep(0.85, 0.95, abs(band));
        alpha *= mix(1.0, 0.15, gap);
        alpha *= 0.9 + 0.2 * hash(vec2(floor(ringDist * 50.0), 0.0));
    } else if (uStyle == 2) {
        // Sparse narrow discrete rings with wide gaps (Uranus-like)
        float p = fract(ringDist * 8.0);
        float visible = smoothstep(0.0, 0.08, p) * smoothstep(0.30, 0.22, p);
        alpha *= visible;
    } else {
        // Faint uniform dust ring (Jupiter / Neptune-like)
        alpha *= 0.5 + 0.5 * hash(vec2(floor(ringDist * 20.0), 0.0));
    }

    vec2 wn = (ringDist > 0.0001) ? (w / ringDist) : vec2(1.0, 0.0);
    float lightTerm = 0.5 + 0.5 * dot(wn, normalize(uLightDir));
    col *= mix(0.55, 1.0, lightTerm);

    fragColor = vec4(col, alpha * edge);
}
)GLSL";

// ── Lazy-initialised shared GL state ──────────────────────────────────────────

namespace {

constexpr float MIN_VISIBLE_PX = 1.0f;

float visualRadiusPx(double physicalRadiusM, double zoom) {
    return std::max((float)(physicalRadiusM * scale * zoom), MIN_VISIBLE_PX);
}

GLuint compileShader(GLenum stage, const char* src) {
    GLuint sh = glCreateShader(stage);
    glShaderSource(sh, 1, &src, nullptr);
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

GLuint linkProgram(const char* vsSrc, const char* fsSrc) {
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

GLuint makeQuadVAO(float half, GLuint& vboOut) {
    GLuint vao;
    float quad[8] = { -half,-half,  half,-half,  -half,half,  half,half };
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

struct PlanetGL {
    GLuint prog = 0, vao = 0, vbo = 0;
    GLint locCenter, locRadius, locScreenW, locScreenH;
    GLint locLightDir, locTilt, locRotAngle, locType, locCoreR;
    GLint locColor1, locColor2, locColor3;
    bool ready = false;
};

PlanetGL& planetGL() {
    static PlanetGL s;
    if (s.ready) return s;

    s.prog = linkProgram(PLANET_VS, PLANET_FS);
    s.vao = makeQuadVAO(1.0f, s.vbo); 

    s.locCenter   = glGetUniformLocation(s.prog, "uCenterPx");
    s.locRadius   = glGetUniformLocation(s.prog, "uRadiusPx");
    s.locScreenW  = glGetUniformLocation(s.prog, "uScreenW");
    s.locScreenH  = glGetUniformLocation(s.prog, "uScreenH");
    s.locLightDir = glGetUniformLocation(s.prog, "uLightDir");
    s.locTilt     = glGetUniformLocation(s.prog, "uTiltRad");
    s.locRotAngle = glGetUniformLocation(s.prog, "uRotAngle");
    s.locType     = glGetUniformLocation(s.prog, "uType");
    s.locCoreR    = glGetUniformLocation(s.prog, "uCoreR");
    s.locColor1   = glGetUniformLocation(s.prog, "uColor1");
    s.locColor2   = glGetUniformLocation(s.prog, "uColor2");
    s.locColor3   = glGetUniformLocation(s.prog, "uColor3");

    s.ready = true;
    return s;
}

struct RingGL {
    GLuint prog = 0, vao = 0, vbo = 0;
    GLint locCenter, locRadius, locScreenW, locScreenH;
    GLint locTilt, locInner, locOuter, locStyle, locBackPass;
    GLint locLightDir, locRingColor, locRingAlpha;
    bool ready = false;
};

RingGL& ringGL() {
    static RingGL s;
    if (s.ready) return s;

    s.prog = linkProgram(RING_VS, RING_FS);
    s.vao = makeQuadVAO(4.0f, s.vbo);

    s.locCenter    = glGetUniformLocation(s.prog, "uCenterPx");
    s.locRadius    = glGetUniformLocation(s.prog, "uRadiusPx");
    s.locScreenW   = glGetUniformLocation(s.prog, "uScreenW");
    s.locScreenH   = glGetUniformLocation(s.prog, "uScreenH");
    s.locTilt      = glGetUniformLocation(s.prog, "uTiltRad");
    s.locInner     = glGetUniformLocation(s.prog, "uInner");
    s.locOuter     = glGetUniformLocation(s.prog, "uOuter");
    s.locStyle     = glGetUniformLocation(s.prog, "uStyle");
    s.locBackPass  = glGetUniformLocation(s.prog, "uBackPass");
    s.locLightDir  = glGetUniformLocation(s.prog, "uLightDir");
    s.locRingColor = glGetUniformLocation(s.prog, "uRingColor");
    s.locRingAlpha = glGetUniformLocation(s.prog, "uRingAlpha");

    s.ready = true;
    return s;
}

void drawRingPass(const Body& body, const Camera& cam, bool backPass) {
    if (!body.hasRings) return;
    RingGL& s = ringGL();

    float sx = (float)(body.pos.x * scale * cam.zoom) + SCR_W * 0.5f + (float)cam.offsetX;
    float sy = (float)(body.pos.y * scale * cam.zoom) + SCR_H * 0.5f + (float)cam.offsetY;

    Vec2 toSun = Vec2(0.0, 0.0) - body.pos;
    double toSunLen = toSun.length();
    float lightDirX = toSunLen > 0.0 ? (float)(toSun.x / toSunLen) : 1.0f;
    float lightDirY = toSunLen > 0.0 ? (float)(toSun.y / toSunLen) : 0.0f;

    glUseProgram(s.prog);
    glUniform2f(s.locCenter, sx, sy);
    glUniform1f(s.locRadius, visualRadiusPx(body.drawRadius, cam.zoom));
    glUniform1f(s.locScreenW, (float)SCR_W);
    glUniform1f(s.locScreenH, (float)SCR_H);
    glUniform1f(s.locTilt, (float)body.axialTilt);
    glUniform1f(s.locInner, body.ringInner);
    glUniform1f(s.locOuter, body.ringOuter);
    glUniform1i(s.locStyle, (int)body.ringStyle);
    glUniform1i(s.locBackPass, backPass ? 1 : 0);
    glUniform2f(s.locLightDir, lightDirX, lightDirY);
    glUniform3f(s.locRingColor, body.ringR, body.ringG, body.ringB);
    glUniform1f(s.locRingAlpha, body.ringAlpha);

    glBindVertexArray(s.vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
}

} 

// ── Body ──────────────────────────────────────────────────────────────────────

Body::Body(Vec2 pos, Vec2 vel, double mass,
           float drawRadius, float r, float g, float b)
    : pos(pos), vel(vel), acc{0, 0},
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

void Body::drawRingsBack(const Camera& cam) const { drawRingPass(*this, cam, true); }
void Body::drawRingsFront(const Camera& cam) const { drawRingPass(*this, cam, false); }

void Body::draw(const Camera& cam) const {
    PlanetGL& s = planetGL();

    float sx = (float)(pos.x * scale * cam.zoom) + SCR_W * 0.5f + (float)cam.offsetX;
    float sy = (float)(pos.y * scale * cam.zoom) + SCR_H * 0.5f + (float)cam.offsetY;

    Vec2 toSun = Vec2(0.0, 0.0) - pos;
    double toSunLen = toSun.length();
    float lightDirX = toSunLen > 0.0 ? (float)(toSun.x / toSunLen) : 1.0f;
    float lightDirY = toSunLen > 0.0 ? (float)(toSun.y / toSunLen) : 0.0f;

    bool isStar = (type == PlanetType::Star);
    float coreR = isStar ? (1.0f / 1.8f) : 1.0f;
    float bodyPx = visualRadiusPx(drawRadius, cam.zoom);
    float quadRadius = isStar ? bodyPx * 1.8f : bodyPx;

    float col2r = c2r >= 0.f ? c2r : r, col2g = c2g >= 0.f ? c2g : g, col2b = c2b >= 0.f ? c2b : b;
    float col3r = c3r >= 0.f ? c3r : r, col3g = c3g >= 0.f ? c3g : g, col3b = c3b >= 0.f ? c3b : b;

    glUseProgram(s.prog);
    glUniform2f(s.locCenter, sx, sy);
    glUniform1f(s.locRadius, quadRadius);
    glUniform1f(s.locScreenW, (float)SCR_W);
    glUniform1f(s.locScreenH, (float)SCR_H);
    glUniform2f(s.locLightDir, lightDirX, lightDirY);
    glUniform1f(s.locTilt, (float)axialTilt);
    glUniform1f(s.locRotAngle, (float)rotationAngle);
    glUniform1i(s.locType, (int)type);
    glUniform1f(s.locCoreR, coreR);
    glUniform3f(s.locColor1, r, g, b);
    glUniform3f(s.locColor2, col2r, col2g, col2b);
    glUniform3f(s.locColor3, col3r, col3g, col3b);

    glBindVertexArray(s.vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0); 
}

void Body::drawTrail(const Camera& cam) const {
    if (trail.size() < 2) return;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < (int)trail.size(); ++i) {
        float t = (float)i / (float)(trail.size() - 1);
        float alpha = t * t;
        glColor4f(r, g, b, alpha * 0.75f);
        float sx = (float)(trail[i].x * scale * cam.zoom) + SCR_W * 0.5f + (float)cam.offsetX;
        float sy = (float)(trail[i].y * scale * cam.zoom) + SCR_H * 0.5f + (float)cam.offsetY;
        glVertex2f(sx, sy);
    }
    glEnd();
}