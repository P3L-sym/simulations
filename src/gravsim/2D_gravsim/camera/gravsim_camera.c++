#include "gravsim_camera.h"
#include "../window/gravsim_window.h"

void RegisterCameraCallbacks(GLFWwindow* window, Camera& camera) {
    glfwSetWindowUserPointer(window, &camera);

    glfwSetScrollCallback(window, [](GLFWwindow* w, double, double yoff) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
        double mx, my;
        glfwGetCursorPos(w, &mx, &my);

        double worldX = (mx - SCR_W * 0.5 - cam->offsetX) / (scale * cam->zoom);
        double worldY = (my - SCR_H * 0.5 - cam->offsetY) / (scale * cam->zoom);

        cam->zoom *= (yoff > 0) ? 1.15 : (1.0 / 1.15);

        cam->offsetX = mx - SCR_W * 0.5 - worldX * scale * cam->zoom;
        cam->offsetY = my - SCR_H * 0.5 - worldY * scale * cam->zoom;
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            cam->dragging = (action == GLFW_PRESS);
            if (cam->dragging)
                glfwGetCursorPos(w, &cam->lastMouseX, &cam->lastMouseY);
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
        if (!cam->dragging) return;
        cam->offsetX += x - cam->lastMouseX;
        cam->offsetY += y - cam->lastMouseY;
        cam->lastMouseX = x;
        cam->lastMouseY = y;
    });
}