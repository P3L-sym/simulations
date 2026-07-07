#pragma once
#include <deque>
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
    GasGiant =2,
    IceGiant = 3,
    IcyMoon = 4
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

    std::deque<Vec2> trail;

    Body(Vec2 pos, Vec2 vel, double mass,
         float drawRadius, float r, float g, float b);

    void configureRotation (double periodSeconds, double tiltDeg, PlanetType t, float c2r_, float c2g_, float c2b_, float c3r_, float c3g_, float c3b_);
    void updateRotation (double dtRealSeconds);
    void draw (const Camera& cam) const;
    void drawTrail(const Camera& cam) const;
};