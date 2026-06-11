#include "gravsim_window.h"
#include <iostream>

bool paused = false;
int focusIndex = -1;

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }
    return glfwCreateWindow(SCR_W, SCR_H, "gravsim", nullptr, nullptr);
}

void SetupOpenGL() {
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glOrtho(0, SCR_W, SCR_H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

void SetupWindowCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int action, int) {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) paused = !paused;
        else if (key == GLFW_KEY_1) focusIndex = 0;   // Sun
        else if (key == GLFW_KEY_2) focusIndex = 1;   // Mercury
        else if (key == GLFW_KEY_3) focusIndex = 2;   // Venus
        else if (key == GLFW_KEY_4) focusIndex = 3;   // Earth
        else if (key == GLFW_KEY_5) focusIndex = 5;   // Mars
        else if (key == GLFW_KEY_6) focusIndex = 8;   // Jupiter
        else if (key == GLFW_KEY_7) focusIndex = 9;   // Saturn
        else if (key == GLFW_KEY_8) focusIndex = 10;  // Uranus
        else if (key == GLFW_KEY_9) focusIndex = 11;  // Neptune
        else if (key == GLFW_KEY_0) focusIndex = -1;  // Free camera
    });
}