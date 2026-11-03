#include "window/gravsim_window.h"
#include "camera/gravsim_camera.h"
#include "bodies/gravsim_bodies.h"
#include "physics/gravsim_physics.h"
#include "compute/gravsim_compute.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <algorithm>

// ── Scene description ─────────────────────────────────────────────────────────
// Bodies are listed in the same order as the 2D sim so the number keys (focus)
// keep pointing at the same planets:
//   0 Sun, 1 Mercury, 2 Venus, 3 Earth, 4 Moon, 5 Mars, 6-7 Mars moons,
//   8 Jupiter, 9-16, 17 Saturn, 18-31, 32 Uranus, 33-37, 38 Neptune, 39-46,
//   47 Pluto, 48 Charon

namespace {

constexpr double PI  = 3.14159265358979323846;
constexpr double DEG = PI / 180.0;
constexpr double DAY = 86400.0;
constexpr double TAU = 2.0 * PI;

struct Def {
    int    parent;      // -1 = orbits the Sun (planet), otherwise index of the parent
    double a;           // planets: distance from Sun (m). Moons: orbital radius (m)
    double v;           // planets: orbital speed (m/s). Moons: ignored (computed)
    double mass;
    float  radius;      // m
    float  r, g, b;
    double incDeg;      // planets only: inclination to the ecliptic
    bool   retrograde;
};

// Planet / moon constructors, to keep the table readable.
Def P(double x, double v, double mass, float radius, float r, float g, float b, double incDeg = 0.0) {
    return { -1, x, v, mass, radius, r, g, b, incDeg, false };
}
Def M(int parent, double rOrbit, double mass, float radius, float r, float g, float b, bool retro = false) {
    return { parent, rOrbit, 0.0, mass, radius, r, g, b, 0.0, retro };
}

// Axial tilts (degrees from the ecliptic normal). Moons orbit in their parent's
// equatorial plane, so the same numbers are used for the orbit normal.
constexpr double TILT_MARS = 25.19, TILT_JUPITER = 3.13, TILT_SATURN = 26.73,
                 TILT_URANUS = 97.77, TILT_NEPTUNE = 28.32, TILT_PLUTO = 122.53;

// Place a body on a circular orbit of radius r around (parentPos, parentVel),
// in the plane with unit normal n, starting at the point nearest +X.
void placeOnOrbit(Body& b, Vec3 parentPos, Vec3 parentVel,
                  Vec3 n, double r, double v, bool retrograde) {
    Vec3 xHat(1.0, 0.0, 0.0);
    Vec3 ref = (std::abs(n.dot(xHat)) > 0.95) ? Vec3(0.0, 1.0, 0.0) : xHat;
    Vec3 rHat = (ref - n * n.dot(ref)).normalized();
    Vec3 tHat = n.cross(rHat) * (retrograde ? -1.0 : 1.0);
    b.pos = parentPos + rHat * r;
    b.vel = parentVel + tHat * v;
}

} // namespace

int main() {
    GLFWwindow* window = StartGLFW();
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    SetupOpenGL();

    Camera camera;
    RegisterCameraCallbacks(window, camera);
    SetupWindowCallbacks(window);

    // ── Body table ───────────────────────────────────────────────────────────
    const int EARTH = 3, MARS = 5, JUPITER = 8, SATURN = 17, URANUS = 32, NEPTUNE = 38, PLUTO = 47;

    std::vector<Def> defs = {
        P(0.0, 0.0, 1.989e30, 6.957e8f, 1.00f, 0.90f, 0.20f),                                          // 0 Sun
        P(5.79e10, 47400.0, 3.285e23, 2.4397e6f, 0.72f, 0.55f, 0.38f, 7.00),                           // 1 Mercury
        P(1.082e11, 35020.0, 4.867e24, 6.0518e6f, 0.90f, 0.70f, 0.40f, 3.39),                          // 2 Venus
        P(1.496e11, 29800.0, 5.972e24, 6.371e6f, 0.20f, 0.50f, 1.00f, 0.00),                           // 3 Earth
        M(EARTH, 3.844e8, 7.342e22, 1.7374e6f, 0.70f, 0.70f, 0.70f),                                   // 4 Moon
        P(2.279e11, 24077.0, 6.390e23, 3.3895e6f, 0.78f, 0.35f, 0.18f, 1.85),                          // 5 Mars
        M(MARS, 9.376e6, 1.0659e16, 1.1267e4f, 0.55f, 0.50f, 0.45f),                                   // 6 Phobos
        M(MARS, 2.3463e7, 1.475e15, 6.2e3f, 0.55f, 0.50f, 0.45f),                                      // 7 Deimos
        P(7.783e11, 13070.0, 1.898e27, 6.9911e7f, 0.80f, 0.60f, 0.40f, 1.30),                          // 8 Jupiter
        M(JUPITER, 1.2800e8, 3.600e16, 2.15e4f, 0.60f, 0.55f, 0.50f),                                  // 9 Metis
        M(JUPITER, 1.2900e8, 2.000e15, 8.2e3f, 0.58f, 0.53f, 0.48f),                                   // 10 Adrastea
        M(JUPITER, 1.8140e8, 2.080e18, 8.35e4f, 0.65f, 0.35f, 0.25f),                                  // 11 Amalthea
        M(JUPITER, 2.2180e8, 4.300e17, 4.93e4f, 0.60f, 0.50f, 0.40f),                                  // 12 Thebe
        M(JUPITER, 4.2160e8, 8.932e22, 1.8216e6f, 1.00f, 0.80f, 0.20f),                                // 13 Io
        M(JUPITER, 6.7090e8, 4.800e22, 1.5608e6f, 0.85f, 0.85f, 0.95f),                                // 14 Europa
        M(JUPITER, 1.0704e9, 1.482e23, 2.6341e6f, 0.55f, 0.52f, 0.48f),                                // 15 Ganymede
        M(JUPITER, 1.8827e9, 1.076e23, 2.4103e6f, 0.38f, 0.36f, 0.34f),                                // 16 Callisto
        P(1.427e12, 9690.0, 5.683e26, 5.8232e7f, 0.90f, 0.80f, 0.50f, 2.49),                           // 17 Saturn
        M(SATURN, 1.3368e8, 4.950e15, 1.41e4f, 0.80f, 0.78f, 0.74f),                                   // 18 Pan
        M(SATURN, 1.3940e8, 1.600e17, 4.31e4f, 0.75f, 0.72f, 0.68f),                                   // 19 Prometheus
        M(SATURN, 1.4170e8, 1.400e17, 4.07e4f, 0.74f, 0.71f, 0.67f),                                   // 20 Pandora
        M(SATURN, 1.5142e8, 5.300e17, 5.81e4f, 0.72f, 0.70f, 0.66f),                                   // 21 Epimetheus
        M(SATURN, 1.5147e8, 1.900e18, 8.95e4f, 0.72f, 0.70f, 0.66f),                                   // 22 Janus
        M(SATURN, 1.8552e8, 3.750e19, 1.982e5f, 0.78f, 0.76f, 0.74f),                                  // 23 Mimas
        M(SATURN, 2.3820e8, 1.080e20, 2.521e5f, 0.98f, 0.98f, 1.00f),                                  // 24 Enceladus
        M(SATURN, 2.9466e8, 6.174e20, 5.311e5f, 0.80f, 0.78f, 0.76f),                                  // 25 Tethys
        M(SATURN, 3.7740e8, 1.095e21, 5.614e5f, 0.78f, 0.76f, 0.74f),                                  // 26 Dione
        M(SATURN, 5.2710e8, 2.307e21, 7.638e5f, 0.76f, 0.74f, 0.72f),                                  // 27 Rhea
        M(SATURN, 1.2218e9, 1.345e23, 2.575e6f, 0.90f, 0.60f, 0.20f),                                  // 28 Titan
        M(SATURN, 1.4818e9, 5.620e18, 1.35e5f, 0.70f, 0.65f, 0.58f),                                   // 29 Hyperion
        M(SATURN, 3.5613e9, 1.806e21, 7.345e5f, 0.60f, 0.58f, 0.55f),                                  // 30 Iapetus
        M(SATURN, 1.2952e10, 8.290e18, 1.065e5f, 0.40f, 0.38f, 0.36f, true),                           // 31 Phoebe (retrograde)
        P(2.871e12, 6810.0, 8.681e25, 2.5362e7f, 0.40f, 0.80f, 0.90f, 0.77),                           // 32 Uranus
        M(URANUS, 1.2993e8, 6.590e19, 2.358e5f, 0.65f, 0.64f, 0.63f),                                  // 33 Miranda
        M(URANUS, 1.9102e8, 1.353e21, 5.789e5f, 0.70f, 0.69f, 0.68f),                                  // 34 Ariel
        M(URANUS, 2.6600e8, 1.172e21, 5.847e5f, 0.40f, 0.39f, 0.38f),                                  // 35 Umbriel
        M(URANUS, 4.3600e8, 3.527e21, 7.884e5f, 0.62f, 0.61f, 0.60f),                                  // 36 Titania
        M(URANUS, 5.8350e8, 3.014e21, 7.614e5f, 0.52f, 0.50f, 0.48f),                                  // 37 Oberon
        P(4.495e12, 5430.0, 1.024e26, 2.4622e7f, 0.20f, 0.40f, 0.90f, 1.77),                           // 38 Neptune
        M(NEPTUNE, 4.8227e7, 1.900e17, 3.3e4f, 0.50f, 0.55f, 0.65f),                                   // 39 Naiad
        M(NEPTUNE, 5.0074e7, 3.500e17, 4.1e4f, 0.50f, 0.55f, 0.65f),                                   // 40 Thalassa
        M(NEPTUNE, 5.2526e7, 2.100e18, 7.5e4f, 0.48f, 0.53f, 0.63f),                                   // 41 Despina
        M(NEPTUNE, 6.1953e7, 3.750e18, 8.8e4f, 0.48f, 0.53f, 0.63f),                                   // 42 Galatea
        M(NEPTUNE, 7.3548e7, 4.200e18, 9.7e4f, 0.48f, 0.53f, 0.63f),                                   // 43 Larissa
        M(NEPTUNE, 1.1765e8, 4.400e19, 2.1e5f, 0.45f, 0.50f, 0.60f),                                   // 44 Proteus
        M(NEPTUNE, 3.5476e8, 2.139e22, 1.3534e6f, 0.75f, 0.65f, 0.65f, true),                          // 45 Triton (retrograde)
        M(NEPTUNE, 5.5134e9, 3.100e19, 1.7e5f, 0.48f, 0.53f, 0.60f),                                   // 46 Nereid
        P(5.906e12, 4743.0, 1.303e22, 1.1883e6f, 0.80f, 0.70f, 0.60f, 17.16),                          // 47 Pluto
        M(PLUTO, 1.9591e7, 1.586e21, 6.06e5f, 0.65f, 0.63f, 0.60f),                                    // 48 Charon
    };

    // Axial tilt of each parent, so moons can borrow it for their orbit plane.
    auto parentTilt = [&](int parent) -> double {
        switch (parent) {
            case MARS:    return TILT_MARS;
            case JUPITER: return TILT_JUPITER;
            case SATURN:  return TILT_SATURN;
            case URANUS:  return TILT_URANUS;
            case NEPTUNE: return TILT_NEPTUNE;
            case PLUTO:   return TILT_PLUTO;
            default:      return 0.0;
        }
    };

    // ── Build the bodies (positions + velocities in 3D) ──────────────────────
    std::vector<Body> bodies;
    bodies.reserve(defs.size());
    std::vector<double> moonR(defs.size(), 0.0), moonV(defs.size(), 0.0);

    for (size_t i = 0; i < defs.size(); ++i) {
        const Def& d = defs[i];
        bodies.emplace_back(Vec3(0, 0, 0), Vec3(0, 0, 0), d.mass, d.radius, d.r, d.g, d.b);
        Body& b = bodies.back();
        b.parent = d.parent;

        if (d.parent < 0) {
            // Planet: circular orbit around the Sun, inclined to the ecliptic.
            double inc = d.incDeg * DEG;
            Vec3 normal(0.0, -std::sin(inc), std::cos(inc));
            if (i != 0) placeOnOrbit(b, Vec3(0, 0, 0), Vec3(0, 0, 0), normal, d.a, d.v, false);
        } else {
            // Moon: circular orbit in the parent's equatorial plane.
            const Body& parent = bodies[d.parent];
            double tilt = parentTilt(d.parent) * DEG;
            Vec3 normal;
            if (d.parent == EARTH) {
                double inc = 5.14 * DEG;                         // the Moon's orbit is tilted to the ecliptic, not to Earth's equator
                normal = Vec3(0.0, -std::sin(inc), std::cos(inc));
            } else {
                normal = Vec3(std::sin(tilt), 0.0, std::cos(tilt));
            }
            double v = std::sqrt(G * parent.mass / d.a);
            moonR[i] = d.a;
            moonV[i] = v;
            placeOnOrbit(b, parent.pos, parent.vel, normal, d.a, v, d.retrograde);
        }
    }

    // Give the Sun the opposite momentum so the whole system doesn't drift away.
    {
        Vec3 p(0, 0, 0);
        for (size_t i = 1; i < bodies.size(); ++i) p += bodies[i].vel * bodies[i].mass;
        bodies[0].vel = p * (-1.0 / bodies[0].mass);
    }

    // ── Rotation / appearance ────────────────────────────────────────────────
    auto tidalLockPeriod = [](double r, double v) { return TAU * r / v; };

    auto autoMoon = [&](int idx, double tiltDeg) {
        double period = tidalLockPeriod(moonR[idx], moonV[idx]);
        if (defs[idx].retrograde) period = -period;
        float pr = bodies[idx].r, pg = bodies[idx].g, pb = bodies[idx].b;
        bodies[idx].configureRotation(
            period, tiltDeg, PlanetType::IcyMoon,
            pr * 0.55f, pg * 0.55f, pb * 0.55f,
            std::min(1.0f, pr + 0.3f), std::min(1.0f, pg + 0.3f), std::min(1.0f, pb + 0.3f)
        );
    };

    // Tidally locked moons spin about their orbit normal.
    for (size_t i = 0; i < defs.size(); ++i) {
        if (defs[i].parent < 0) continue;
        autoMoon((int)i, defs[i].parent == EARTH ? 1.54 : parentTilt(defs[i].parent));
    }

    // Sun and planets
    bodies[0].configureRotation(25.05 * DAY, 7.25, PlanetType::Star, 1.00f, 0.55f, 0.05f, 1.00f, 1.00f, 0.85f);
    bodies[1].configureRotation(58.646 * DAY, 0.03, PlanetType::Terrestrial, 0.35f, 0.32f, 0.30f, 0.75f, 0.72f, 0.68f);
    bodies[2].configureRotation(-243.025 * DAY, 177.4, PlanetType::Terrestrial, 0.75f, 0.60f, 0.35f, 0.95f, 0.88f, 0.65f);
    bodies[3].configureRotation(0.99727 * DAY, 23.44, PlanetType::Terrestrial, 0.25f, 0.50f, 0.20f, 0.95f, 0.95f, 1.00f);
    bodies[5].configureRotation(1.025957 * DAY, TILT_MARS, PlanetType::Terrestrial, 0.45f, 0.20f, 0.10f, 0.90f, 0.85f, 0.80f);
    bodies[8].configureRotation(0.41354 * DAY, TILT_JUPITER, PlanetType::GasGiant, 0.65f, 0.45f, 0.30f, 0.95f, 0.85f, 0.75f);
    bodies[17].configureRotation(0.44401 * DAY, TILT_SATURN, PlanetType::GasGiant, 0.75f, 0.65f, 0.45f, 0.98f, 0.92f, 0.80f);
    bodies[32].configureRotation(-0.71833 * DAY, TILT_URANUS, PlanetType::IceGiant, 0.40f, 0.70f, 0.75f, 0.75f, 0.95f, 0.95f);
    bodies[38].configureRotation(0.6713 * DAY, TILT_NEPTUNE, PlanetType::IceGiant, 0.15f, 0.25f, 0.65f, 0.55f, 0.65f, 0.95f);
    bodies[47].configureRotation(6.3872 * DAY, TILT_PLUTO, PlanetType::Terrestrial, 0.55f, 0.45f, 0.40f, 0.90f, 0.88f, 0.85f);

    // Rings
    bodies[8].configureRings(1.4f, 1.9f, RingStyle::Faint, 0.55f, 0.50f, 0.45f, 0.10f);   // Jupiter
    bodies[17].configureRings(1.25f, 2.3f, RingStyle::Broad, 0.85f, 0.78f, 0.60f, 0.65f); // Saturn
    bodies[32].configureRings(1.6f, 2.4f, RingStyle::Narrow, 0.45f, 0.48f, 0.52f, 0.55f); // Uranus
    bodies[38].configureRings(1.7f, 2.1f, RingStyle::Faint, 0.35f, 0.40f, 0.55f, 0.12f);  // Neptune

    // ── GPU setup ────────────────────────────────────────────────────────────
    GravCompute gpu;
    gpu.init((int)bodies.size());
    gpu.upload(bodies);

    std::printf(
        "Controls:\n"
        "  left-drag   orbit          right-drag  pan\n"
        "  scroll      zoom           1-9         follow Sun/Mercury/Venus/Earth/Mars/Jupiter/Saturn/Uranus/Neptune\n"
        "  0           free camera    R           reset view\n"
        "  [ / ]       slower/faster  (hold Shift for x10)\n"
        "  Space       pause          Esc         quit\n");

    camera.sunWorld = bodies[0].pos * scale;
    int lastFocus = -1;
    double accumulator = 0.0;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double now     = glfwGetTime();
        double elapsed = now - lastTime;
        lastTime = now;
        if (elapsed > 0.25) elapsed = 0.25;

        if (!paused) {
            for (auto& b : bodies) b.updateRotation(elapsed * speed_multiplier);

            accumulator += elapsed;

            bool stepped = false;
            while (accumulator >= fixed_real_dt) {
                const double sub_dt = (fixed_real_dt * speed_multiplier) / sub_steps;

                for (int s = 0; s < sub_steps; ++s)
                    gpu.integrateStep(sub_dt);

                accumulator -= fixed_real_dt;
                stepped = true;
            }

            if (stepped) {
                gpu.download(bodies);
                for (auto& b : bodies)
                    b.trail.push(b.parent >= 0 ? b.pos - bodies[b.parent].pos : b.pos);
            }
        }

        // ── Camera ───────────────────────────────────────────────────────────
        if (resetViewRequested) {
            camera.resetView();
            focusIndex = -1;
            resetViewRequested = false;
        }

        if (focusIndex != lastFocus) {
            if (focusIndex >= 0 && focusIndex < (int)bodies.size()) {
                // Frame the newly selected body (rings included).
                const Body& f = bodies[focusIndex];
                double extent = (double)f.drawRadius * scale * (f.hasRings ? f.ringOuter : 1.0f);
                camera.distance = std::max(extent * 6.0, camera.minDistance);
            }
            lastFocus = focusIndex;
        }
        if (focusIndex >= 0 && focusIndex < (int)bodies.size())
            camera.target = bodies[focusIndex].pos * scale;

        camera.sunWorld = bodies[0].pos * scale;
        camera.update(fbWidth, fbHeight);

        // ── Draw ─────────────────────────────────────────────────────────────
        // Order: stars -> opaque bodies (write depth) -> glow, rings, trails
        // (translucent: depth-tested but not depth-written).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawStarfield(camera);

        for (auto& b : bodies) b.draw(camera);

        glDepthMask(GL_FALSE);
        for (auto& b : bodies) b.drawGlow(camera);
        for (auto& b : bodies) b.drawRings(camera);
        for (auto& b : bodies)
            b.drawTrail(camera, b.parent >= 0 ? bodies[b.parent].pos : Vec3(0, 0, 0));
        glDepthMask(GL_TRUE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
