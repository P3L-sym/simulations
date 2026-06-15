#pragma once
#include <GLFW/glfw3.h>

struct Camera {
    double offsetX = 0;
    double offsetY = 0;
    double zoom = 1.0;
    bool dragging = false;
    double lastMouseX = 0;
    double lastMouseY = 0;
};

void RegisterCameraCallbacks(GLFWwindow* window, Camera& camera);