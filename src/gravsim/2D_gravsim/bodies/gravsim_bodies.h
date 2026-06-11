#pragma once
#include <GLFW/glfw3.h>
#include <deque>
#include <cmath>

struct Vec2 {
    double x = 0, y = 0;

    Vec2() = default;
    Vec2(double x, double y) : x(x), y(y) {}

    Vec2  operator+ (Vec2 o) const { return {x + o.x, y + o.y}; }
    Vec2  operator- (Vec2 o) const { return {x - o.x, y - o.y}; }
    Vec2  operator* (double s) const { return {x * s,  y * s  }; }
    Vec2& operator+=(Vec2 o)        { x += o.x; y += o.y; return *this; }

    double dot(Vec2 o)  const { return x * o.x + y * o.y; }
    double length()     const { return std::sqrt(dot(*this)); }
};

struct Camera;

struct Body {
    Vec2   pos;
    Vec2   vel;
    Vec2   acc;
    double mass;
    float  drawRadius;
    float  r, g, b;

    std::deque<Vec2> trail;

    Body(Vec2 pos, Vec2 vel, double mass,
         float drawRadius, float r, float g, float b);

    void draw     (const Camera& cam) const;
    void drawTrail(const Camera& cam) const;
};