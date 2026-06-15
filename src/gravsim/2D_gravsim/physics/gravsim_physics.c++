#include "gravsim_physics.h"
#include "../window/gravsim_window.h"
#include <cmath>

Vec2 accelFrom(const Body& a, const Body& b) {
    Vec2 delta = b.pos - a.pos;
    double dist2 = delta.dot(delta) + (softening * softening);
    double dist = std::sqrt(dist2);
    double factor = G * b.mass / (dist2 * dist);
    return delta * factor;
}

static void kickPos(std::vector<Body>& bodies, double c) {
    for (auto& b : bodies) b.pos += b.vel * c;
}

static void kickVel(std::vector<Body>& bodies, double d) {
    for (int i = 0; i < (int)bodies.size(); ++i) {
        Vec2 a{0,0};
        for(int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) a += accelFrom(bodies[i], bodies[j]);
        bodies[i].vel += a * d;
        bodies[i].acc = a;
    }
}

void integrate(std::vector<Body>& bodies, double dt) {
    static const double cbrt2 = std::cbrt(2.0);
    static const double w1 = 1.0 / (2.0 - cbrt2);
    static const double w0 = -cbrt2 * w1;
    static const double c1 = w1 * 0.5;
    static const double c2 = (w0 + w1) * 0.5;
    static const double d1 = w1;
    static const double d2 = w0;

    kickPos(bodies, c1 * dt);
    kickVel(bodies, d1 * dt);
    kickPos(bodies, c2 * dt);
    kickVel(bodies, d2 * dt);
    kickPos(bodies, c2 * dt);
    kickVel(bodies, d1 * dt);
    kickPos(bodies, c1 * dt);
}