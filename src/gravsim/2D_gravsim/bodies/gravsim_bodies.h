#pragma once
#include <vector>
#include <cmath>

struct Vec2 {
    double x = 0, y = 0;

    Vec2() = default;
    Vec2(double x, double y) : x(x), y(y) {}

    Vec2 operator+ (Vec2 o) const { return {x + o.x, y + o.y}; }
    Vec2 operator- (Vec2 o) const { return {x - o.x, y - o.y}; }
    Vec2 operator* (double s) const { return {x * s,  y * s  }; }
    Vec2& operator+=(Vec2 o) { x += o.x; y += o.y; return *this; }

    double dot(Vec2 o) const { return x * o.x + y * o.y; }
    double length() const { return std::sqrt(dot(*this)); }
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
    std::vector<double> points;
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

    void push(Vec2 p);
    void draw(const Camera& cam, float r, float g, float b) const;
};

struct Body {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    double mass;
    float drawRadius;
    float r, g, b;

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

    Body(Vec2 pos, Vec2 vel, double mass,
         float drawRadius, float r, float g, float b);

    void configureRotation(double periodSeconds, double tiltDeg, PlanetType t,
                            float c2r_, float c2g_, float c2b_,
                            float c3r_, float c3g_, float c3b_);

    void configureRings(float innerMult, float outerMult, RingStyle style,
                         float rr, float rg, float rb, float alpha);

    void updateRotation(double dtRealSeconds);

    void drawRingsBack(const Camera& cam) const;
    void draw(const Camera& cam) const;
    void drawRingsFront(const Camera& cam) const;

    void drawTrail(const Camera& cam) const;
};