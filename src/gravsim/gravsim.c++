#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

const int SCR_W = 800;
const int SCR_H = 600;
const double G  = 6.674e-11;
const double softening = 1e8;
const double scale = 1.5e-9;
const double time_scale = 3.0e5; 

struct Vec2{
    double x = 0, y = 0;

    Vec2() = default;
    Vec2(double x, double y) : x(x), y(y) {}

    Vec2 operator+(Vec2 o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(Vec2 o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(double s) const { return {x * s, y * s  }; }
    Vec2& operator+=(Vec2 o) { x += o.x; y += o.y; return * this; }

    double dot(Vec2 o) const { return x * o.x + y * o.y; }
    double length() const {return std::sqrt(dot(*this)); }
};

struct Body {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;
    double mass;
    float drawRadius;

    Body( Vec2 pos, Vec2 vel, double mass, float drawRadius = 5.0f)
        : pos(pos), vel(vel), acc{0,0}, mass(mass), drawRadius(drawRadius) {}

    void draw() const {
        float sx = (float)(pos.x * scale) + SCR_W * 0.5f;
        float sy = (float)(pos.y * scale) + SCR_H * 0.5f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int i = 0; i <= 64; i++){
            float a = 2.0f * 3.14159265f * i / 64.0f;
            glVertex2f(sx + std::cos(a) * drawRadius,
                       sy + std::sin(a) * drawRadius);
        }
        glEnd();
    }
};

Vec2 accelFrom(const Body& a, const Body& b) {
    Vec2 delta = b.pos - a.pos;
    double dist2 = delta.dot(delta) + (softening * softening);
    double dist = std::sqrt(dist2);
    double factor = G * b.mass / (dist2 * dist);
    return delta * factor;
}

void integrate(std::vector<Body>& bodies, double dt) {
    for(auto& b : bodies)
        b.pos += b.vel * dt + b.acc * (0.5 * dt * dt);
    
    for (int i = 0; i < (int)bodies.size(); ++i) {
        Vec2 newAcc{0, 0};
        for (int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) newAcc += accelFrom(bodies[i], bodies[j]);

        bodies[i].vel += (bodies[i].acc + newAcc) * (0.5 * dt);
        bodies[i].acc = newAcc;
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

int main(){
    GLFWwindow *window = StartGLFW();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCR_W, SCR_H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::vector<Body> bodies = {
        Body({ 0.0, 0.0 }, { 0.0, 0.0 }, 1.989e30, 15.0f),        // Sol
        Body({ 1.5e11, 0.0 }, { 0.0, 29800.0 }, 5.972e24, 5.0f),  // Earth
    };
    
    for (int i = 0; i < (int)bodies.size(); ++i)
        for (int j = 0; j < (int)bodies.size(); ++j)
            if (i != j) bodies[i].acc += accelFrom(bodies[i], bodies[j]);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)){
        double now = glfwGetTime();
        double simDt = (now - lastTime) * time_scale;
        lastTime = now;

        integrate(bodies, simDt);

        glClear(GL_COLOR_BUFFER_BIT);
        for (auto& b : bodies) b.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}