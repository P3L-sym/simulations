#include "gravsim_physics.h"
#include "../window/gravsim_window.h"
#include <cmath>

Vec2 accelFrom(const Body& a, const Body& b) {
    Vec2   delta  = b.pos - a.pos;
    double dist2  = delta.dot(delta) + (softening * softening);
    double dist   = std::sqrt(dist2);
    double factor = G * b.mass / (dist2 * dist);
    return delta * factor;
}

void integrate(std::vector<Body>& bodies, double dt) {
    for (auto& b : bodies)
        b.pos += b.vel * dt + b.acc * (0.5 * dt * dt);

    for (int i = 0; i < (int)bodies.size(); ++i) {
        Vec2 newAcc{0, 0};
        for (int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) newAcc += accelFrom(bodies[i], bodies[j]);

        bodies[i].vel += (bodies[i].acc + newAcc) * (0.5 * dt);
        bodies[i].acc  = newAcc;
    }
}