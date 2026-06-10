#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

const int    SCR_W = 800;
const int    SCR_H = 600;
const double G = 6.674e-11;
const double softening = 0.0;
const double scale = 1.5e-9;
const double time_scale = 3.0e5;
bool paused = false;
int focusIndex = -1;

struct Vec2 {
    double x = 0, y = 0;

    Vec2() = default;
    Vec2(double x, double y) : x(x), y(y) {}

    Vec2  operator+ (Vec2 o) const { return {x + o.x, y + o.y}; }
    Vec2  operator- (Vec2 o) const { return {x - o.x, y - o.y}; }
    Vec2  operator* (double s) const { return {x * s,   y * s  }; }
    Vec2& operator+=(Vec2 o) { x += o.x; y += o.y; return * this; }

    double dot(Vec2 o) const { return x * o.x + y * o.y; }
    double length() const { return std::sqrt(dot(*this)); }
};

struct Camera {
    double offsetX = 0, offsetY = 0;
    double zoom = 1.0;
    bool dragging = false;
    double lastMouseX = 0, lastMouseY = 0;
};

struct Body {
    Vec2   pos;
    Vec2   vel;
    Vec2   acc;
    double mass;
    float  drawRadius;
    float r, g, b;

    Body(Vec2 pos, Vec2 vel, double mass, float drawRadius, float r, float g, float b)
        : pos(pos), vel(vel), acc{0,0}, mass(mass), drawRadius(drawRadius), r(r), g(g), b(b) {}

    void draw(const Camera& cam) const {
        float sx = (float)(pos.x * scale * cam.zoom) + SCR_W * 0.5f + (float)cam.offsetX;
        float sy = (float)(pos.y * scale * cam.zoom) + SCR_H * 0.5f + (float)cam.offsetY;

        glColor3f(r, g, b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int i = 0; i <= 64; ++i) {
            float a = 2.0f * 3.14159265f * i / 64.0f;
            glVertex2f(sx + std::cos(a) * drawRadius,
                       sy + std::sin(a) * drawRadius);
        }
        glEnd();
    }
};

Vec2 accelFrom(const Body& a, const Body& b) {
    Vec2   delta   = b.pos - a.pos;
    double dist2   = delta.dot(delta) + (softening * softening);
    double dist    = std::sqrt(dist2);
    double factor  = G * b.mass / (dist2 * dist);
    return delta * factor;
}

void integrate(std::vector<Body>& bodies, double dt) {
    for (auto& b : bodies)
        b.pos += b.vel * dt + b.acc * (0.5 * dt * dt);

    for (int i = 0; i < (int)bodies.size(); ++i) {
        Vec2 newAcc{0, 0};
        for (int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) newAcc += accelFrom(bodies[i], bodies[j]);

        bodies[i].vel += (bodies[i].acc + newAcc) * (0.5 * dt);
        bodies[i].acc  = newAcc;
    }
}

GLFWwindow *StartGLFW(){
    if (!glfwInit()){
        std::cerr << "Failed to initialize glfw" << std::endl;
        return nullptr;
    }

    GLFWwindow *window = glfwCreateWindow(SCR_W, SCR_H, "gravsim", nullptr, nullptr);

    return window;
}

int main() {
    GLFWwindow *window = StartGLFW();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, SCR_W, SCR_H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

    Camera camera;
    glfwSetWindowUserPointer(window, &camera);

    glfwSetScrollCallback(window, [](GLFWwindow * w, double, double yoff){
        Camera * cam = (Camera*)glfwGetWindowUserPointer(w);
        double mx, my;
        glfwGetCursorPos(w, &mx, &my);

        double worldX = (mx - SCR_W * 0.5 - cam->offsetX) / (scale * cam->zoom);
        double worldY = (my - SCR_H * 0.5 - cam->offsetY) / (scale * cam->zoom);

        cam->zoom *= (yoff > 0) ? 1.15 : (1.0 / 1.15);

        cam->offsetX = mx - SCR_W * 0.5 - worldX * scale * cam->zoom;
        cam->offsetY = my - SCR_H * 0.5 - worldY * scale * cam->zoom;
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int) {
        Camera* cam = (Camera*)glfwGetWindowUserPointer(w);
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            cam->dragging = (action == GLFW_PRESS);
            if (cam->dragging)
                glfwGetCursorPos(w, &cam->lastMouseX, &cam->lastMouseY);
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Camera* cam = (Camera*)glfwGetWindowUserPointer(w);
        if (!cam->dragging) return;
        cam->offsetX   += x - cam->lastMouseX;
        cam->offsetY   += y - cam->lastMouseY;
        cam->lastMouseX = x;
        cam->lastMouseY = y;
    });

    glfwSetKeyCallback(window, [](GLFWwindow * w, int key, int, int action, int) {
        if (key == GLFW_KEY_SPACE && action ==GLFW_PRESS)
            paused = !paused;
        else if (key == GLFW_KEY_1) { focusIndex = 0; }
        else if (key == GLFW_KEY_2) { focusIndex = 1; }
        else if (key == GLFW_KEY_3) { focusIndex = 2; }
        else if (key == GLFW_KEY_4) { focusIndex = 3; }
        else if (key == GLFW_KEY_5) { focusIndex = 5; }
        else if (key == GLFW_KEY_6) { focusIndex = 8; }
        else if (key == GLFW_KEY_7) { focusIndex = 9; }
        else if (key == GLFW_KEY_8) { focusIndex = 10; }
        else if (key == GLFW_KEY_9) { focusIndex = 11; }
        else if (key == GLFW_KEY_0) { focusIndex = -1; }
    });

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

    const double M_Earth = std::sqrt(G * Earth_Mass / 3.844e8); 
    const double P_Mars = std::sqrt(G * Mars_Mass / 9.376e6);
    const double D_Mars = std::sqrt(G * Mars_Mass / 2.346e7);

    std::vector<Body> bodies = {
        Body({ 0.0, 0.0 }, { 0.0, 0.0 }, Sun_Mass, 15.0f, 1.00f, 0.90f, 0.20f),                                   // Sun       0
        Body({ 5.79e10, 0.0 }, { 0.0, 47400.0 }, Mercury_Mass, 3.0f, 0.72f, 0.55f, 0.38f),                        // Mercury   1 
        Body({ 1.082e11, 0.0 }, { 0.0, 35020.0 }, Venus_Mass, 4.5f, 0.90f, 0.70f, 0.40f),                         // Venus     2
        Body({ 1.496e11, 0.0 }, { 0.0, 29800.0 }, Earth_Mass, 5.0f, 0.20f, 0.50f, 1.00f),                         // Earth     3
        Body({ 1.496e11 + 3.844e8, 0.0 }, { 0.0, 29800.0 + M_Earth }, Moon_Mass, 3.0f, 0.70f, 0.70f, 0.70f),      // Moon      4
        Body({ 2.279e11, 0.0 }, { 0.0, 24077.0 }, Mars_Mass, 4.0f, 0.78f, 0.35f, 0.18f),                          // Mars      5
        Body({ 2.279e11 + 9.376e6, 0.0 }, { 0.0, 24077.0 + P_Mars }, Phobos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),     //Phobos     6
        Body({ 2.279e11 + 2.346e7, 0.0 }, { 0.0, 24077.0 + D_Mars }, Deimos_Mass, 1.5f, 0.55f, 0.50f, 0.45f),     //Deimos     7
        Body({ 7.783e11, 0.0 }, { 0.0, 13070.0 }, Jupiter_Mass, 10.0f, 0.80f, 0.60f, 0.40f),                      // Jupiter   8
        Body({ 1.427e12, 0.0 }, { 0.0, 9690.0 }, Saturn_Mass, 9.0f, 0.90f, 0.80f, 0.50f),                         // Saturn    9 
        Body({ 2.871e12, 0.0 }, { 0.0, 6810.0 }, Uranus_Mass, 7.0f, 0.40f, 0.80f, 0.90f),                         // Uranus    10
        Body({ 4.495e12, 0.0 }, { 0.0, 5430.0 }, Neptune_Mass, 7.0f, 0.20f, 0.40f, 0.90f),                        // Neptune   11
    };

    for (int i = 0; i < (int)bodies.size(); ++i)
        for (int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) bodies[i].acc += accelFrom(bodies[i], bodies[j]);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double simDt = (now - lastTime) * time_scale;
        lastTime = now;

        if(!paused)
            integrate(bodies, simDt);

        if (focusIndex >= 0 && focusIndex < (int)bodies.size()) {
            camera.offsetX = -(bodies[focusIndex].pos.x * scale * camera.zoom);
            camera.offsetY = -(bodies[focusIndex].pos.y * scale * camera.zoom);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        for (auto& b : bodies) b.draw(camera);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}