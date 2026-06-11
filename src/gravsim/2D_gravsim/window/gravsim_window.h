#pragma once
#include <GLFW/glfw3.h>

inline constexpr int SCR_W = 1920;
inline constexpr int SCR_H = 1080;
inline constexpr int trail_length = 3600;
inline constexpr int sub_steps = 10;
inline constexpr double G = 6.674e-11;
inline constexpr double softening = 0.0;
inline constexpr double scale = 1.5e-9;
inline constexpr double time_scale = 3.0e5;

extern bool paused;
extern int  focusIndex;

GLFWwindow* StartGLFW();
void SetupOpenGL();
void SetupWindowCallbacks(GLFWwindow* window);