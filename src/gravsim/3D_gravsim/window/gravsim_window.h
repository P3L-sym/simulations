#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

inline constexpr int SCR_W = 1280;
inline constexpr int SCR_H = 720;
inline constexpr int trail_length = 3600;
inline constexpr int sub_steps = 10;
inline constexpr double G = 6.674e-11;
inline constexpr double softening = 0.0;
// metres -> scene units. 1 AU is ~224 scene units, Neptune ~6700.
inline constexpr double scale = 1.5e-9;
inline constexpr double fixed_real_dt = 1.0 / 120.0;

extern double speed_multiplier;
extern bool paused;
extern int focusIndex;
extern bool resetViewRequested;
extern int fbWidth;
extern int fbHeight;

GLFWwindow* StartGLFW();
void SetupOpenGL();
void SetupWindowCallbacks(GLFWwindow* window);
