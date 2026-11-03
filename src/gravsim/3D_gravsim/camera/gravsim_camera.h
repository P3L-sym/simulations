#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../bodies/gravsim_bodies.h"

// Orbit camera. It always looks at `target` from `distance` away, at angles
// (yaw, pitch). World +Z is "up" (the ecliptic normal).
//
// All camera quantities are in *scene units* (= metres * `scale`).
struct Camera {
    // ── controlled by the user / main loop ───────────────────────────────────
    double yaw = 0.6;
    double pitch = 0.55;
    double distance = 12000.0;        // whole solar system in view
    double minDistance = 1.0e-6;
    double maxDistance = 3.0e4;
    Vec3   target{0.0, 0.0, 0.0};
    double fovY = 50.0 * 3.14159265358979323846 / 180.0;
    Vec3   sunWorld{0.0, 0.0, 0.0};   // light source, scene units

    bool   orbiting = false;          // left mouse
    bool   panning = false;           // right / middle mouse
    double lastMouseX = 0, lastMouseY = 0;

    // ── derived each frame by update() ───────────────────────────────────────
    Vec3   eye{0.0, 0.0, 0.0};
    Vec3   right{1.0, 0.0, 0.0};
    Vec3   up{0.0, 0.0, 1.0};
    Vec3   forward{0.0, 1.0, 0.0};
    int    viewW = 1, viewH = 1;
    double aspect = 1.0;
    double focal = 1.0;               // 1 / tan(fovY / 2)
    double focalPx = 1.0;             // focal length in pixels

    void resetView();
    void update(int w, int h);

    // scene position -> view space (camera at origin looking down -Z).
    // The subtraction happens in double, *then* it is narrowed to float on the
    // way to the GPU. This is the "floating origin" trick.
    Vec3 toView(const Vec3& scenePos) const;
    Vec3 dirToView(const Vec3& worldDir) const;
};

void RegisterCameraCallbacks(GLFWwindow* window, Camera& camera);
