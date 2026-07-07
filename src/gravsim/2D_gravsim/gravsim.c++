#include "window/gravsim_window.h"
#include "camera/gravsim_camera.h"
#include "bodies/gravsim_bodies.h"
#include "physics/gravsim_physics.h"
#include "compute/gravsim_compute.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <algorithm>

int main() {
    GLFWwindow* window = StartGLFW();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    SetupOpenGL();  

    Camera camera;
    RegisterCameraCallbacks(window, camera);
    SetupWindowCallbacks(window);

    // ────Parent Planet Position and Velocities────
    const double x_earth = 1.496e11, v_earth = 29800.0;
    const double x_mars = 2.279e11 , v_mars = 24077.0;
    const double x_jupiter = 7.783e11, v_jupiter = 13070.0;
    const double x_saturn = 1.427e12, v_saturn = 9690.0;
    const double x_uranus = 2.871e12, v_uranus = 6810.0;
    const double x_neptune = 4.495e12, v_neptune = 5430.0;
    const double x_pluto = 5.906e12, v_pluto = 4743.0;

    // ────Celestial Bodies Masses────
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
    const double Prometheus_Mass = 1.600e17;
    const double Pandora_Mass = 1.400e17;
    const double Epimetheus_Mass = 5.300e17;
    const double Janus_Mass = 1.900e18;
    const double Mimas_Mass = 3.750e19;
    const double Enceladus_Mass = 1.080e20;
    const double Tethys_Mass = 6.174e20;
    const double Dione_Mass = 1.095e21;
    const double Rhea_Mass = 2.307e21;
    const double Titan_Mass = 1.345e23;
    const double Hyperion_Mass = 5.620e18;
    const double Iapetus_Mass = 1.806e21;
    const double Phoebe_Mass = 8.290e18;
    const double Uranus_Mass = 8.681e25;
    const double Miranda_Mass = 6.590e19;
    const double Ariel_Mass = 1.353e21;
    const double Umbriel_Mass = 1.172e21;
    const double Titania_Mass = 3.527e21;
    const double Oberon_Mass = 3.014e21;
    const double Neptune_Mass = 1.024e26;
    const double Naiad_Mass = 1.900e17;
    const double Thalassa_Mass = 3.500e17;
    const double Despina_Mass = 2.100e18;
    const double Galatea_Mass = 3.750e18;
    const double Larissa_Mass = 4.200e18;
    const double Proteus_Mass = 4.400e19;
    const double Triton_Mass = 2.139e22;
    const double Nereid_Mass = 3.100e19;
    const double Pluto_Mass = 1.303e22;
    const double Charon_Mass = 1.586e21;

    const double GM_earth = G * Earth_Mass;
    const double GM_mars = G * Mars_Mass;
    const double GM_jupiter = G * Jupiter_Mass;
    const double GM_saturn = G * Saturn_Mass;
    const double GM_uranus = G * Uranus_Mass;
    const double GM_neptune = G * Neptune_Mass;
    const double GM_pluto = G * Pluto_Mass;

    // ────Orbital Radii────
    // Earth
    const double r_moon = 3.844e8;
    // Mars
    const double r_phobos = 9.376e6;
    const double r_deimos = 2.3463e7;
    // Jupiter
    const double r_metis = 1.2800e8;
    const double r_adrastea = 1.2900e8;
    const double r_amalthea = 1.8140e8;
    const double r_thebe = 2.2180e8;
    const double r_io = 4.2160e8;
    const double r_europa = 6.7090e8;
    const double r_ganymede = 1.0704e9;
    const double r_callisto = 1.8827e9;
    // Saturn
    const double r_pan = 1.3368e8;
    const double r_prometheus = 1.3940e8;
    const double r_pandora = 1.4170e8;
    const double r_epimetheus = 1.5142e8;
    const double r_janus = 1.5147e8;
    const double r_mimas = 1.8552e8;
    const double r_enceladus = 2.3820e8;
    const double r_tethys = 2.9466e8;
    const double r_dione = 3.7740e8;
    const double r_rhea = 5.2710e8;
    const double r_titan = 1.2218e9;
    const double r_hyperion = 1.4818e9;
    const double r_iapetus = 3.5613e9;
    const double r_phoebe = 1.2952e10; // retrograde
    // Uranus
    const double r_miranda = 1.2993e8;
    const double r_ariel = 1.9102e8;
    const double r_umbriel = 2.6600e8;
    const double r_titania = 4.3600e8;
    const double r_oberon = 5.8350e8;
    // Neptune 
    const double r_naiad = 4.8227e7;
    const double r_thalassa = 5.0074e7;
    const double r_despina = 5.2526e7;
    const double r_galatea = 6.1953e7;
    const double r_larissa = 7.3548e7;
    const double r_proteus = 1.1765e8;
    const double r_triton = 3.5476e8; // retrograde
    const double r_nereid = 5.5134e9;
    // Pluto 
    const double r_charon = 1.9591e7;

    // ────Orbital Velocities────
    // Earth
    const double v_moon = std::sqrt(GM_earth / r_moon);
    // Mars
    const double v_phobos = std::sqrt(GM_mars / r_phobos);
    const double v_deimos = std::sqrt(GM_mars / r_deimos);
    // Jupite
    const double v_metis = std::sqrt(GM_jupiter / r_metis);
    const double v_adrastea = std::sqrt(GM_jupiter / r_adrastea);
    const double v_amalthea = std::sqrt(GM_jupiter / r_amalthea);
    const double v_thebe = std::sqrt(GM_jupiter / r_thebe);
    const double v_io = std::sqrt(GM_jupiter / r_io);
    const double v_europa = std::sqrt(GM_jupiter / r_europa);
    const double v_ganymede = std::sqrt(GM_jupiter / r_ganymede);
    const double v_callisto = std::sqrt(GM_jupiter / r_callisto);
    // Saturn
    const double v_pan = std::sqrt(GM_saturn / r_pan);
    const double v_prometheus = std::sqrt(GM_saturn / r_prometheus);
    const double v_pandora = std::sqrt(GM_saturn / r_pandora);
    const double v_epimetheus = std::sqrt(GM_saturn / r_epimetheus);
    const double v_janus = std::sqrt(GM_saturn / r_janus);
    const double v_mimas = std::sqrt(GM_saturn / r_mimas);
    const double v_enceladus = std::sqrt(GM_saturn / r_enceladus);
    const double v_tethys = std::sqrt(GM_saturn / r_tethys);
    const double v_dione = std::sqrt(GM_saturn  / r_dione);
    const double v_rhea = std::sqrt(GM_saturn / r_rhea);
    const double v_titan = std::sqrt(GM_saturn / r_titan);
    const double v_hyperion = std::sqrt(GM_saturn / r_hyperion);
    const double v_iapetus = std::sqrt(GM_saturn / r_iapetus);
    const double v_phoebe = std::sqrt(GM_saturn / r_phoebe);
    // Uranus
    const double v_miranda = std::sqrt(GM_uranus / r_miranda);
    const double v_ariel = std::sqrt(GM_uranus / r_ariel);
    const double v_umbriel = std::sqrt(GM_uranus / r_umbriel);
    const double v_titania = std::sqrt(GM_uranus / r_titania);
    const double v_oberon = std::sqrt(GM_uranus / r_oberon);
    // Neptune 
    const double v_naiad = std::sqrt(GM_neptune / r_naiad);
    const double v_thalassa = std::sqrt(GM_neptune / r_thalassa);
    const double v_despina = std::sqrt(GM_neptune / r_despina);
    const double v_galatea = std::sqrt(GM_neptune / r_galatea);
    const double v_larissa = std::sqrt(GM_neptune / r_larissa);
    const double v_proteus = std::sqrt(GM_neptune / r_proteus);
    const double v_triton = std::sqrt(GM_neptune / r_triton);
    const double v_nereid = std::sqrt(GM_neptune / r_nereid);
    // Pluto
    const double v_charon = std::sqrt(GM_pluto / r_charon);

    std::vector<Body> bodies = {
        Body({ 0.0, 0.0 }, { 0.0, 0.0 }, Sun_Mass, 15.0f, 1.00f, 0.90f, 0.20f),                                                       // 0 Sun
        Body({ 5.79e10, 0.0 }, { 0.0, 47400.0 }, Mercury_Mass, 3.0f, 0.72f, 0.55f, 0.38f),                                            // 1 Mercury
        Body({ 1.082e11, 0.0 }, { 0.0, 35020.0 }, Venus_Mass, 4.5f, 0.90f, 0.70f, 0.40f),                                             // 2 Venus
        Body({ x_earth, 0.0 }, { 0.0, v_earth }, Earth_Mass, 5.0f, 0.20f, 0.50f, 1.00f),                                              // 3 Earth
        Body({ x_earth + r_moon, 0.0 }, { 0.0, v_earth + v_moon }, Moon_Mass, 3.0f, 0.70f, 0.70f, 0.70f),                             // 4 Moon
        Body({ x_mars, 0.0 }, { 0.0, 24077.0 }, Mars_Mass, 4.0f, 0.78f, 0.35f, 0.18f),                                                // 5 Mars
        Body({ x_mars + r_phobos, 0.0 }, { 0.0, v_mars + v_phobos }, Phobos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),                         // 6 Phobos
        Body({ x_mars + r_deimos, 0.0 }, { 0.0, v_mars + v_deimos }, Deimos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),                         // 7 Deimos
        Body({ x_jupiter, 0.0 }, { 0.0, v_jupiter }, Jupiter_Mass, 10.0f, 0.80f, 0.60f, 0.40f),                                       // 8 Jupiter
        Body({ x_jupiter + r_metis, 0.0 }, { 0.0, v_jupiter + v_metis }, Metis_Mass, 1.0f, 0.60f, 0.55f, 0.50f),                      // 9 Metis
        Body({ x_jupiter + r_adrastea, 0.0 }, { 0.0, v_jupiter + v_adrastea }, Adrastea_Mass, 1.0f, 0.58f, 0.53f, 0.48f),             // 10 Adrastea
        Body({ x_jupiter + r_amalthea, 0.0 }, { 0.0, v_jupiter + v_amalthea }, Amalthea_Mass, 1.5f, 0.65f, 0.35f, 0.25f),             // 11 Amalthea
        Body({ x_jupiter + r_thebe, 0.0 }, { 0.0, v_jupiter + v_thebe }, Thebe_Mass, 1.0f, 0.60f, 0.50f, 0.40f),                      // 12 Thebe
        Body({ x_jupiter + r_io, 0.0 }, { 0.0, v_jupiter + v_io }, Io_Mass, 3.0f, 1.00f, 0.80f, 0.20f),                               // 13 Io
        Body({ x_jupiter + r_europa, 0.0 }, { 0.0, v_jupiter + v_europa }, Europa_Mass, 2.5f, 0.85f, 0.85f, 0.95f),                   // 14 Europa
        Body({ x_jupiter + r_ganymede, 0.0 }, { 0.0, v_jupiter + v_ganymede }, Ganymede_Mass, 3.5f, 0.55f, 0.52f, 0.48f),             // 15 Ganymede
        Body({ x_jupiter + r_callisto, 0.0 }, { 0.0, v_jupiter + v_callisto }, Callisto_Mass, 3.0f, 0.38f, 0.36f, 0.34f),             // 16 Callisto
        Body({ x_saturn, 0.0 }, { 0.0, v_saturn }, Saturn_Mass, 9.0f, 0.90f, 0.80f, 0.50f),                                           // 17 Saturn
        Body({ x_saturn + r_pan, 0.0 }, { 0.0, v_saturn + v_pan }, Pan_Mass, 1.0f, 0.80f, 0.78f, 0.74f),                              // 18 Pan
        Body({ x_saturn + r_prometheus, 0.0 }, { 0.0, v_saturn + v_prometheus }, Prometheus_Mass, 1.0f, 0.75f, 0.72f, 0.68f),         // 19 Prometheus
        Body({ x_saturn + r_pandora, 0.0 }, { 0.0, v_saturn + v_pandora }, Pandora_Mass, 1.0f, 0.74f, 0.71f, 0.67f),                  // 20 Pandora
        Body({ x_saturn + r_epimetheus, 0.0 }, { 0.0, v_saturn + v_epimetheus }, Epimetheus_Mass, 1.0f, 0.72f, 0.70f, 0.66f),         // 21 Epimetheus
        Body({ x_saturn + r_janus, 0.0 }, { 0.0, v_saturn + v_janus }, Janus_Mass, 1.0f, 0.72f, 0.70f, 0.66f),                        // 22 Janus
        Body({ x_saturn + r_mimas, 0.0 }, { 0.0, v_saturn + v_mimas }, Mimas_Mass, 1.5f, 0.78f, 0.76f, 0.74f),                        // 23 Mimas
        Body({ x_saturn + r_enceladus, 0.0 }, { 0.0, v_saturn + v_enceladus }, Enceladus_Mass, 1.5f, 0.98f, 0.98f, 1.00f),            // 24 Enceladus
        Body({ x_saturn + r_tethys, 0.0 }, { 0.0, v_saturn + v_tethys }, Tethys_Mass, 2.0f, 0.80f, 0.78f, 0.76f),                     // 25 Tethys
        Body({ x_saturn + r_dione, 0.0 }, { 0.0, v_saturn + v_dione }, Dione_Mass, 2.0f, 0.78f, 0.76f, 0.74f),                        // 26 Dione
        Body({ x_saturn + r_rhea, 0.0 }, { 0.0, v_saturn + v_rhea }, Rhea_Mass, 2.0f, 0.76f, 0.74f, 0.72f),                           // 27 Rhea
        Body({ x_saturn + r_titan, 0.0 }, { 0.0, v_saturn + v_titan }, Titan_Mass, 3.5f, 0.90f, 0.60f, 0.20f),                        // 28 Titan
        Body({ x_saturn + r_hyperion, 0.0 }, { 0.0, v_saturn + v_hyperion }, Hyperion_Mass, 1.5f, 0.70f, 0.65f, 0.58f),               // 29 Hyperion
        Body({ x_saturn + r_iapetus, 0.0 }, { 0.0, v_saturn + v_iapetus }, Iapetus_Mass, 2.0f, 0.60f, 0.58f, 0.55f),                  // 30 Iapetus
        Body({ x_saturn + r_phoebe, 0.0 }, { 0.0, v_saturn - v_phoebe }, Phoebe_Mass, 1.0f, 0.40f, 0.38f, 0.36f),                     // 31 Phoebe (Retrograde)
        Body({ x_uranus, 0.0 }, { 0.0, v_uranus }, Uranus_Mass, 7.0f, 0.40f, 0.80f, 0.90f),                                           // 32 Uranus
        Body({ x_uranus + r_miranda, 0.0 }, { 0.0, v_uranus + v_miranda }, Miranda_Mass, 1.5f, 0.65f, 0.64f, 0.63f),                  // 33 Miranda
        Body({ x_uranus + r_ariel, 0.0 }, { 0.0, v_uranus + v_ariel }, Ariel_Mass, 2.0f, 0.70f, 0.69f, 0.68f),                        // 34 Ariel
        Body({ x_uranus + r_umbriel, 0.0 }, { 0.0, v_uranus + v_umbriel }, Umbriel_Mass, 2.0f, 0.40f, 0.39f, 0.38f),                  // 35 Umbriel
        Body({ x_uranus + r_titania, 0.0 }, { 0.0, v_uranus + v_titania }, Titania_Mass, 2.5f, 0.62f, 0.61f, 0.60f),                  // 36 Titania
        Body({ x_uranus + r_oberon, 0.0 }, { 0.0, v_uranus + v_oberon }, Oberon_Mass, 2.5f, 0.52f, 0.50f, 0.48f),                     // 37 Oberon
        Body({ x_neptune, 0.0 }, { 0.0, v_neptune }, Neptune_Mass, 7.0f, 0.20f, 0.40f, 0.90f),                                        // 38 Neptune 
        Body({ x_neptune + r_naiad, 0.0 }, { 0.0, v_neptune + v_naiad }, Naiad_Mass, 1.0f, 0.50f, 0.55f, 0.65f),                      // 39 Naiad
        Body({ x_neptune + r_thalassa, 0.0 }, { 0.0, v_neptune + v_thalassa }, Thalassa_Mass, 1.0f, 0.50f, 0.55f, 0.65f),             // 40 Thalassa
        Body({ x_neptune + r_despina, 0.0 }, { 0.0, v_neptune + v_despina }, Despina_Mass, 1.0f, 0.48f, 0.53f, 0.63f),                // 41 Despina
        Body({ x_neptune + r_galatea, 0.0 }, { 0.0, v_neptune + v_galatea }, Galatea_Mass, 1.0f, 0.48f, 0.53f, 0.63f),                // 42 Galatea
        Body({ x_neptune + r_larissa, 0.0 }, { 0.0, v_neptune + v_larissa }, Larissa_Mass, 1.0f, 0.48f, 0.53f, 0.63f),                // 43 Larissa 
        Body({ x_neptune + r_proteus, 0.0 }, { 0.0, v_neptune + v_proteus }, Proteus_Mass, 1.5f, 0.45f, 0.50f, 0.60f),                // 44 Proteus
        Body({ x_neptune + r_triton, 0.0 }, { 0.0, v_neptune - v_triton }, Triton_Mass, 2.5f, 0.75f, 0.65f, 0.65f),                   // 45 Triton (Retrograde)
        Body({ x_neptune + r_nereid, 0.0 }, { 0.0, v_neptune + v_nereid }, Nereid_Mass, 1.0f, 0.48f, 0.53f, 0.60f),                   // 46 Nereid
        Body({ x_pluto, 0.0 }, { 0.0, v_pluto }, Pluto_Mass, 2.5f, 0.80f, 0.70f, 0.60f),                                              // 47 Pluto
        Body({ x_pluto + r_charon, 0.0 }, { 0.0, v_pluto + v_charon }, Charon_Mass, 2.0f, 0.65f, 0.63f, 0.60f),                       // 48 Charon
    };

    constexpr double DAY = 86400.0;
    constexpr double TAU = 6.283185307179586;
    auto tidalLockPeriod = [](double r, double v) { return TAU * r / v; };

    auto autoMoon = [&](int idx, double r, double v, bool retrograde = false, double tiltDeg = 1.0) {
        double period = tidalLockPeriod(r, v);
        if(retrograde) period = -period;
        float pr = bodies[idx].r, pg = bodies[idx].g, pb = bodies[idx].b;
        bodies[idx].configureRotation(
            period, tiltDeg, PlanetType::IcyMoon,
            pr * 0.55f, pg * 0.55f, pb * 0.55f,
            std::min(1.0f, pr + 0.3f), std::min(1.0f, pg + 0.3f), std::min(1.0f, pb + 0.3f)
        );
    };

    // Sun
    bodies[0].configureRotation(25.05 * DAY, 7.25, PlanetType::Star, 1.00f, 0.55f, 0.05f, 1.00f, 1.00f, 0.85f);

    // Mercury / Venus / Earth (+Moon) / Mars (+Phobos, Deimos)
    bodies[1].configureRotation(58.646 * DAY, 0.03, PlanetType::Terrestrial, 0.35f, 0.32f, 0.30f,   0.75f, 0.72f, 0.68f);
 

    // ── GPU setup ────────────────────────────────────────────────────────────
    GravCompute gpu;
    gpu.init((int)bodies.size());
    gpu.upload(bodies);

    double accumulator = 0.0;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double now     = glfwGetTime();
        double elapsed = now - lastTime;
        lastTime = now;
        if (elapsed > 0.25) elapsed = 0.25;

        if (!paused) {
            accumulator += elapsed;

            while (accumulator >= fixed_real_dt) {
                const double sub_dt = (fixed_real_dt * speed_multiplier) / sub_steps;

                // All sub-steps run on the GPU — no CPU↔GPU transfers inside
                for (int s = 0; s < sub_steps; ++s)
                    gpu.integrateStep(sub_dt);

                // Single readback per physics tick, just for trail positions
                gpu.download(bodies);

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