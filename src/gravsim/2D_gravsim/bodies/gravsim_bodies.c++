#include "gravsim_bodies.h"
#include "../camera/gravsim_camera.h"
#include "../window/gravsim_window.h"
#include <cmath>

Body::Body(Vec2 pos, Vec2 vel, double mass,
           float drawRadius, float r, float g, float b)
    : pos(pos), vel(vel), acc{0, 0},
      mass(mass), drawRadius(drawRadius),
      r(r), g(g), b(b) {}

void Body::draw(const Camera& cam) const {
    float sx = (float)(pos.x * scale * cam.zoom) + SCR_W * 0.5f + (float)cam.offsetX;
    float sy = (float)(pos.y * scale * cam.zoom) + SCR_H * 0.5f + (float)cam.offsetY;

    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(sx, sy);
    for (int i = 0; i <= 64; ++i) {
        float a = 2.0f * 3.14159265f * i / 64.0f;
        glVertex2f(sx + std::cos(a) * drawRadius,
                   sy + std::sin(a) * drawRadius);
    }
    glEnd();
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