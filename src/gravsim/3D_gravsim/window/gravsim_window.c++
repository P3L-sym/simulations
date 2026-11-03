#include "gravsim_window.h"
#include <algorithm>
#include <iostream>

bool paused = false;
int focusIndex = -1;
bool resetViewRequested = false;
double speed_multiplier = 86400.0;   // 1 simulated day per real second
int fbWidth = SCR_W;
int fbHeight = SCR_H;

// With sub_steps = 10 and a 120 Hz fixed step, one integrator sub-step covers
// speed / 1200 simulated seconds. The innermost moons orbit in ~7 hours
// (25,000 s), so past ~1e6x they get fewer than ~20 steps per orbit and the
// integrator can no longer follow them. Raise sub_steps if you want more.
static constexpr double MAX_SPEED = 1048576.0;
static constexpr double MIN_SPEED = 1.0 / 64.0;

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, 4);   // MSAA: smooths trail lines
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    return glfwCreateWindow(SCR_W, SCR_H, "gravsim 3D", nullptr, nullptr);
}

void SetupOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cerr << "Failed to initialise GLAD\n";

    GLFWwindow* win = glfwGetCurrentContext();
    glfwGetFramebufferSize(win, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void SetupWindowCallbacks(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        fbWidth = w;
        fbHeight = h;
        if (w > 0 && h > 0) glViewport(0, 0, w, h);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int, int action, int mods) {
        if (action != GLFW_PRESS) return;

        if (key == GLFW_KEY_SPACE) paused = !paused;
        else if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(win, GLFW_TRUE);
        else if (key == GLFW_KEY_R) resetViewRequested = true;
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

        else if (key == GLFW_KEY_RIGHT_BRACKET || key == GLFW_KEY_LEFT_BRACKET) {
            // ] faster, [ slower. Hold Shift for x10 instead of x2.
            double factor = (mods & GLFW_MOD_SHIFT) ? 10.0 : 2.0;
            if (key == GLFW_KEY_LEFT_BRACKET) factor = 1.0 / factor;
            speed_multiplier = std::clamp(speed_multiplier * factor, MIN_SPEED, MAX_SPEED);
            std::cout << "Speed: " << speed_multiplier << "x\n";
        }
    });
}
