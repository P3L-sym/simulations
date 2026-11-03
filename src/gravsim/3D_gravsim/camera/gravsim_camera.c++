#include "gravsim_camera.h"
#include "../window/gravsim_window.h"

#include <algorithm>
#include <cmath>

// ── Camera maths ──────────────────────────────────────────────────────────────

void Camera::resetView() {
    yaw = 0.6;
    pitch = 0.55;
    distance = 12000.0;
    target = Vec3(0.0, 0.0, 0.0);
}

void Camera::update(int w, int h) {
    viewW = std::max(w, 1);
    viewH = std::max(h, 1);
    aspect = (double)viewW / (double)viewH;

    constexpr double PITCH_LIMIT = 1.55; // just shy of 90° so `up` never degenerates
    pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT);
    distance = std::clamp(distance, minDistance, maxDistance);

    // Direction from the target to the eye.
    double cp = std::cos(pitch), sp = std::sin(pitch);
    Vec3 dir(cp * std::cos(yaw), cp * std::sin(yaw), sp);

    eye     = target + dir * distance;
    forward = -dir;
    right   = forward.cross(Vec3(0.0, 0.0, 1.0)).normalized();
    up      = right.cross(forward);

    focal   = 1.0 / std::tan(fovY * 0.5);
    focalPx = 0.5 * viewH * focal;
}

Vec3 Camera::toView(const Vec3& p) const {
    Vec3 rel = p - eye;
    return Vec3(rel.dot(right), rel.dot(up), -rel.dot(forward));
}

Vec3 Camera::dirToView(const Vec3& d) const {
    return Vec3(d.dot(right), d.dot(up), -d.dot(forward));
}

// ── Input ─────────────────────────────────────────────────────────────────────
//   left-drag  : orbit
//   right-drag : pan (drops out of "follow body" mode)
//   scroll     : zoom toward the target

void RegisterCameraCallbacks(GLFWwindow* window, Camera& camera) {
    glfwSetWindowUserPointer(window, &camera);

    glfwSetScrollCallback(window, [](GLFWwindow* w, double, double yoff) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
        cam->distance *= (yoff > 0) ? (1.0 / 1.15) : 1.15;
        cam->distance = std::clamp(cam->distance, cam->minDistance, cam->maxDistance);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
        bool down = (action == GLFW_PRESS);
        if (button == GLFW_MOUSE_BUTTON_LEFT) cam->orbiting = down;
        if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_MIDDLE) cam->panning = down;
        if (down) glfwGetCursorPos(w, &cam->lastMouseX, &cam->lastMouseY);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
        double dx = x - cam->lastMouseX;
        double dy = y - cam->lastMouseY;
        cam->lastMouseX = x;
        cam->lastMouseY = y;

        if (cam->orbiting) {
            cam->yaw   -= dx * 0.005;
            cam->pitch += dy * 0.005;
        } else if (cam->panning) {
            focusIndex = -1; // panning detaches from whatever we were following
            double unitsPerPixel = 2.0 * cam->distance * std::tan(cam->fovY * 0.5) / std::max(fbHeight, 1);
            cam->target += cam->up * (dy * unitsPerPixel) - cam->right * (dx * unitsPerPixel);
        }
    });
}
