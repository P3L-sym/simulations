#include "gravsim_window.h"
#include <iostream>

bool paused = false;
int focusIndex = -1;
double speed_multiplier = 1.0;

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    return glfwCreateWindow(mode->width, mode->height, "gravsim", nullptr, nullptr);
}

void SetupOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        std::cerr << "Failed to initialise GLAD\n";
    
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glOrtho(0, SCR_W, SCR_H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

void SetupWindowCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int, int action, int) {
        if (action != GLFW_PRESS) return;

        if (key == GLFW_KEY_SPACE) paused = !paused;
        else if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(win, GLFW_TRUE);
        else if (key == GLFW_KEY_1) focusIndex = 0;   // Sun
        else if (key == GLFW_KEY_2) focusIndex = 1;   // Mercury
        else if (key == GLFW_KEY_3) focusIndex = 2;   // Venus
        else if (key == GLFW_KEY_4) focusIndex = 3;   // Earth
        else if (key == GLFW_KEY_5) focusIndex = 5;   // Mars
        else if (key == GLFW_KEY_6) focusIndex = 8;   // Jupiter
        else if (key == GLFW_KEY_7) focusIndex = 17;  // Saturn
        else if (key == GLFW_KEY_8) focusIndex = 32;  // Uranus
        else if (key == GLFW_KEY_9) focusIndex = 38;  // Neptune
        else if (key == GLFW_KEY_0) focusIndex = -1;  // Free camera

        else if (key == GLFW_KEY_RIGHT_BRACKET) {
            speed_multiplier *= 2.0;
            if (speed_multiplier < 1.0 * 64.0) speed_multiplier = 1.0 * 64.0;
            std::cout << "Speed: " << speed_multiplier <<  "x\n";
        }
        else if (key == GLFW_KEY_LEFT_BRACKET) {
            speed_multiplier /= 2.0;
            if (speed_multiplier < 1.0 / 64.0 ) speed_multiplier = 1.0 / 64.0;
            std::cout << "Speed: " << speed_multiplier <<  "x\n";
        }
    });
}