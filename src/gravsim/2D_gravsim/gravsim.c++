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

    const double x_earth = 1.496e11, v_earth = 29800.0;
    const double x_mars = 2.279e11 , v_mars = 24077.0;
    const double x_jupiter = 7.783e11, v_jupiter = 13070.0;
    const double x_saturn = 1.427e12, v_saturn = 9690.0;
    const double x_uranus = 2.871e12, v_uranus = 6810.0;
    const double x_neptune = 4.495e12, v_neptune = 5430.0;

    const double Sun_Mass = 1.989e30;
    const double Mercury_Mass = 3.285e23;
    const double Venus_Mass = 4.867e24;
    
    const double Earth_Mass = 5.972e24;
    const double Moon_Mass = 7.342e22;
    
    const double Mars_Mass = 6.390e23;
    const double Phobos_Mass = 1.0659e16;
    const double Deimos_Mass = 1.475e15;

    const double Jupiter_Mass = 1.898e27;
    const double Metis_Mass = 3.600e16;
    const double Adrastea_Mass = 2.000e15;
    const double Amalthea_Mass = 2.080e18;
    const double Thebe_Mass = 4.300e17;
    const double Io_Mass = 8.932e22;
    const double Europa_Mass = 4.800e22;
    const double Ganymede_Mass = 1.482e23;
    const double Callisto_Mass = 1.076e23;

    const double Saturn_Mass = 5.683e26;
    const double Pan_Mass = 4.950e15;

    const double Uranus_Mass = 8.681e25;
    
    const double Neptune_Mass = 1.024e26;

    const double GM_earth = G * Earth_Mass;
    const double GM_mars = G * Mars_Mass;
    const double GM_jupiter = G * Jupiter_Mass;
    const double GM_saturn = G * Saturn_Mass;
    const double GM_uranus = G * Uranus_Mass;
    const double GM_neptune = G * Neptune_Mass;

    //earth
    const double r_moon = 3.844e8;

    //mars
    const double r_phobos = 9.376e6;
    const double r_deimos = 2.3463e7;

    //jupiter
    const double r_metis = 1.2800e8;
    const double r_adrastea = 1.2900e8;
    const double r_amalthea = 1.8140e8;
    const double r_thebe = 2.2180e8;
    const double r_io = 4.2160e8;
    const double r_europa = 6.7090e8;
    const double r_ganymede = 1.0704e9;
    const double r_callisto = 1.8827e9;

    //saturn
    const double r_pan = 1.3368e8;

    const double v_moon = std::sqrt(GM_earth / r_moon);   

    const double v_phobos = std::sqrt(GM_mars / r_phobos);   
    const double v_deimos = std::sqrt(GM_mars / r_deimos); 
    
    const double v_metis = std::sqrt(GM_jupiter / r_metis);   
    const double v_adrastea = std::sqrt(GM_jupiter / r_adrastea);  
    const double v_amalthea = std::sqrt(GM_jupiter / r_amalthea);  
    const double v_thebe = std::sqrt(GM_jupiter / r_thebe);   
    const double v_io = std::sqrt(GM_jupiter / r_io);  
    const double v_europa = std::sqrt(GM_jupiter / r_europa);  
    const double v_ganymede = std::sqrt(GM_jupiter / r_ganymede);  
    const double v_callisto = std::sqrt(GM_jupiter / r_callisto);  

    const double v_pan = std::sqrt(GM_saturn / r_pan); 

    std::vector<Body> bodies = {
        Body({ 0.0, 0.0 }, { 0.0, 0.0 }, Sun_Mass, 15.0f, 1.00f, 0.90f, 0.20f),                                              // 0  Sun
        Body({ 5.79e10, 0.0 }, { 0.0, 47400.0 }, Mercury_Mass, 3.0f, 0.72f, 0.55f, 0.38f),                                   // 1  Mercury
        Body({ 1.082e11, 0.0 }, { 0.0, 35020.0 }, Venus_Mass, 4.5f, 0.90f, 0.70f, 0.40f),                                    // 2  Venus
        Body({ x_earth, 0.0 }, { 0.0, v_earth }, Earth_Mass, 5.0f, 0.20f, 0.50f, 1.00f),                                     // 3  Earth
        Body({ x_earth + r_moon, 0.0 }, { 0.0, v_earth + v_moon }, Moon_Mass, 3.0f, 0.70f, 0.70f, 0.70f),                    // 4  Moon
        Body({ x_mars, 0.0 }, { 0.0, 24077.0 }, Mars_Mass, 4.0f, 0.78f, 0.35f, 0.18f),                                       // 5  Mars
        Body({ x_mars + r_phobos, 0.0 }, { 0.0, v_mars + v_phobos }, Phobos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),                // 6  Phobos
        Body({ x_mars + r_deimos, 0.0 }, { 0.0, v_mars + v_deimos }, Deimos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),                // 7  Deimos
        Body({ x_jupiter, 0.0 }, { 0.0, v_jupiter }, Jupiter_Mass, 10.0f, 0.80f, 0.60f, 0.40f),                              // 8  Jupiter
        Body({ x_jupiter + r_metis, 0.0 }, { 0.0, v_jupiter + v_metis }, Metis_Mass, 1.0f, 0.60f, 0.55f, 0.50f),             // 9  Metis
        Body({ x_jupiter + r_adrastea, 0.0 }, { 0.0, v_jupiter + v_adrastea }, Adrastea_Mass, 1.0f, 0.58f, 0.53f, 0.48f),    // 10 Adrastea
        Body({ x_jupiter + r_amalthea, 0.0 }, { 0.0, v_jupiter + v_amalthea }, Amalthea_Mass, 1.5f, 0.65f, 0.35f, 0.25f),    // 11 Amalthea
        Body({ x_jupiter + r_thebe, 0.0 }, { 0.0, v_jupiter + v_thebe }, Thebe_Mass, 1.0f, 0.60f, 0.50f, 0.40f),             // 12 Thebe
        Body({ x_jupiter + r_io, 0.0 }, { 0.0, v_jupiter + v_io }, Io_Mass, 3.0f, 1.00f, 0.80f, 0.20f),                      // 13 Io
        Body({ x_jupiter + r_europa, 0.0 }, { 0.0, v_jupiter + v_europa }, Europa_Mass, 2.5f, 0.85f, 0.85f, 0.95f),          // 14 Europa
        Body({ x_jupiter + r_ganymede, 0.0 }, { 0.0, v_jupiter + v_ganymede }, Ganymede_Mass, 3.5f, 0.55f, 0.52f, 0.48f),    // 15 Ganymede
        Body({ x_jupiter + r_callisto, 0.0 }, { 0.0, v_jupiter + v_callisto }, Callisto_Mass, 3.0f, 0.38f, 0.36f, 0.34f),    // 16 Callisto
        Body({ x_saturn, 0.0 }, { 0.0, v_saturn }, Saturn_Mass, 9.0f, 0.90f, 0.80f, 0.50f),                                  // 17  Saturn
        Body({ x_saturn + r_pan, 0.0 }, { 0.0, v_saturn + v_pan }, Pan_Mass, 1.0f, 0.80f, 0.78f, 0.74f),                     // 18 Pan
        Body({ x_uranus, 0.0 }, { 0.0, v_uranus }, Uranus_Mass, 7.0f, 0.40f, 0.80f, 0.90f),                                  // 19 Uranus
        Body({ x_neptune, 0.0 }, { 0.0, v_neptune }, Neptune_Mass, 7.0f, 0.20f, 0.40f, 0.90f),                               // 20 Neptune
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