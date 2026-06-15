#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

#include "window/gravsim_window.h"
#include "camera/gravsim_camera.h"
#include "bodies/gravsim_bodies.h"
#include "physics/gravsim_physics.h"

int main() {
    GLFWwindow* window = StartGLFW();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    SetupOpenGL();

    Camera camera;
    RegisterCameraCallbacks(window, camera);
    SetupWindowCallbacks(window);

    const double Sun_Mass = 1.989e30;
    const double Mercury_Mass = 3.285e23;
    const double Venus_Mass = 4.867e24;
    const double Earth_Mass = 5.972e24;
    const double Moon_Mass = 7.342e22;
    const double Mars_Mass = 6.390e23;
    const double Phobos_Mass = 1.0659e16;
    const double Deimos_Mass = 1.475e15;
    const double Jupiter_Mass = 1.898e27;
    const double Saturn_Mass = 5.683e26;
    const double Uranus_Mass = 8.681e25;
    const double Neptune_Mass = 1.024e26;

    const double M_Earth = std::sqrt(G * Earth_Mass / 3.844e8);   // Moon
    const double P_Mars = std::sqrt(G * Mars_Mass / 9.376e6);    // Phobos
    const double D_Mars = std::sqrt(G * Mars_Mass / 2.346e7);    // Deimos

    std::vector<Body> bodies = {
        Body({ 0.0, 0.0 }, { 0.0, 0.0 }, Sun_Mass, 15.0f, 1.00f, 0.90f, 0.20f),                                 // 0  Sun
        Body({ 5.79e10, 0.0 }, { 0.0, 47400.0 }, Mercury_Mass, 3.0f, 0.72f, 0.55f, 0.38f),                      // 1  Mercury
        Body({ 1.082e11, 0.0 }, { 0.0, 35020.0 }, Venus_Mass, 4.5f, 0.90f, 0.70f, 0.40f),                       // 2  Venus
        Body({ 1.496e11, 0.0 }, { 0.0, 29800.0 }, Earth_Mass, 5.0f, 0.20f, 0.50f, 1.00f),                       // 3  Earth
        Body({ 1.496e11+3.844e8,0.0 }, { 0.0, 29800.0+M_Earth }, Moon_Mass, 3.0f, 0.70f, 0.70f, 0.70f),         // 4  Moon
        Body({ 2.279e11, 0.0 }, { 0.0, 24077.0 }, Mars_Mass, 4.0f, 0.78f, 0.35f, 0.18f),                        // 5  Mars
        Body({ 2.279e11+9.376e6,0.0 }, { 0.0, 24077.0+P_Mars }, Phobos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),        // 6  Phobos
        Body({ 2.279e11+2.346e7,0.0 }, { 0.0, 24077.0+D_Mars }, Deimos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),        // 7  Deimos
        Body({ 7.783e11, 0.0 }, { 0.0, 13070.0 }, Jupiter_Mass, 10.0f, 0.80f, 0.60f, 0.40f),                    // 8  Jupiter
        Body({ 1.427e12, 0.0 }, { 0.0, 9690.0 }, Saturn_Mass, 9.0f, 0.90f, 0.80f, 0.50f),                       // 9  Saturn
        Body({ 2.871e12, 0.0 }, { 0.0, 6810.0 }, Uranus_Mass, 7.0f, 0.40f, 0.80f, 0.90f),                       // 10 Uranus
        Body({ 4.495e12, 0.0 }, { 0.0, 5430.0 }, Neptune_Mass, 7.0f, 0.20f, 0.40f, 0.90f),                      // 11 Neptune
    };

    for (int i = 0; i < (int)bodies.size(); ++i)
        for (int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) bodies[i].acc += accelFrom(bodies[i], bodies[j]);

    double accumulator = 0.0;   
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double elapsed = now - lastTime;
        lastTime = now;

        if(elapsed > 0.25) elapsed = 0.25;

        if (!paused) {
            accumulator += elapsed;
            
            while (accumulator >= fixed_real_dt) {
                const double sub_dt = (fixed_real_dt * speed_multiplier) / sub_steps;

                for (int s = 0; s < sub_steps; ++s)
                    integrate(bodies, sub_dt);

                for (auto& b : bodies) {
                    b.trail.push_back(b.pos);
                    if ((int)b.trail.size() > trail_length)
                        b.trail.pop_front();
                }
                accumulator -= fixed_real_dt;
            }
        }

        if (focusIndex >= 0 && focusIndex < (int)bodies.size()) {
            camera.offsetX = -(bodies[focusIndex].pos.x * scale * camera.zoom);
            camera.offsetY = -(bodies[focusIndex].pos.y * scale * camera.zoom);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        for (auto& b : bodies) b.drawTrail(camera);
        for (auto& b : bodies) b.draw(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}