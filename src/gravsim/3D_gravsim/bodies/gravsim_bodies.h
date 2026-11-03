#pragma once
#include <vector>
#include <cmath>

// ── Vec3 ──────────────────────────────────────────────────────────────────────
// Everything in the simulation lives in double precision. The GPU only ever
// sees *camera-relative* floats (see Camera::toView), which is what keeps a
// 1e13 m wide solar system from falling apart in 32-bit floats.

struct Vec3 {
    double x = 0, y = 0, z = 0;

    Vec3() = default;
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+ (Vec3 o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec3 operator- (Vec3 o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec3 operator* (double s) const { return {x * s, y * s, z * s}; }
    Vec3 operator- () const { return {-x, -y, -z}; }
    Vec3& operator+=(Vec3 o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec3& operator-=(Vec3 o) { x -= o.x; y -= o.y; z -= o.z; return *this; }

    double dot(Vec3 o) const { return x * o.x + y * o.y + z * o.z; }
    Vec3 cross(Vec3 o) const {
        return { y * o.z - z * o.y,
                 z * o.x - x * o.z,
                 x * o.y - y * o.x };
    }
    double length() const { return std::sqrt(dot(*this)); }
    Vec3 normalized() const {
        double l = length();
        return l > 0.0 ? *this * (1.0 / l) : Vec3(0.0, 0.0, 1.0);
    }
};

struct Camera;

enum class PlanetType {
    Star = 0,
    Terrestrial = 1,
    GasGiant = 2,
    IceGiant = 3,
    IcyMoon = 4
};

enum class RingStyle {
    None = 0,
    Broad = 1,
    Narrow = 2,
    Faint = 3
};

struct Trail {
    std::vector<double> points;   // xyz triplets, ring buffer
    int capacity = 0;
    int head = 0;
    int count = 0;
    unsigned int vao = 0;
    unsigned int vbo = 0;
    bool glReady = false;

    Trail() = default;

    Trail(const Trail& o);
    Trail& operator=(const Trail& o);
    ~Trail();

    // Points are stored relative to the body's parent (so a moon's trail is a
    // closed loop around its planet, not a corkscrew around the Sun). `origin`
    // is the parent's *current* position, added back at draw time.
    void push(Vec3 p);
    void draw(const Camera& cam, float r, float g, float b, Vec3 origin) const;
};

struct Body {
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    double mass;
    float drawRadius;      // metres
    float r, g, b;
    int parent = -1;       // index of the body this one orbits (trail frame); -1 = world origin

    // Spin. The spin axis is tilted `axialTilt` away from the +Z axis (the
    // normal of the ecliptic plane), leaning toward +X. A positive period
    // spins counter-clockwise when viewed from above the north pole, so
    // retrograde rotators like Venus/Uranus get that from their tilt (> 90°).
    double rotationPeriod = 1.0e12;
    double axialTilt = 0.0;
    double rotationAngle = 0.0;
    PlanetType type = PlanetType::IcyMoon;
    float c2r = -1, c2g = -1, c2b = -1;
    float c3r = -1, c3g = -1, c3b = -1;

    bool hasRings = false;
    float ringInner = 0.0f, ringOuter = 0.0f;
    RingStyle ringStyle = RingStyle::None;
    float ringR = 1.0f, ringG = 1.0f, ringB = 1.0f, ringAlpha = 0.5f;

    Trail trail;

    Body(Vec3 pos, Vec3 vel, double mass,
         float drawRadius, float r, float g, float b);

    void configureRotation(double periodSeconds, double tiltDeg, PlanetType t,
                            float c2r_, float c2g_, float c2b_,
                            float c3r_, float c3g_, float c3b_);

    void configureRings(float innerMult, float outerMult, RingStyle style,
                         float rr, float rg, float rb, float alpha);

    Vec3 spinAxis() const { return Vec3(std::sin(axialTilt), 0.0, std::cos(axialTilt)); }

    void updateRotation(double dtRealSeconds);

    // Draw order matters (see gravsim.c++):
    //   starfield -> draw() for all -> drawGlow() -> drawRings() -> drawTrail()
    // draw() writes depth; the others are translucent and only read it.
    void draw(const Camera& cam) const;
    void drawGlow(const Camera& cam) const;
    void drawRings(const Camera& cam) const;
    void drawTrail(const Camera& cam, Vec3 origin) const;
};

void drawStarfield(const Camera& cam);
