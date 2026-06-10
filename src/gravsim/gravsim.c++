#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

float screenHeight = 600.0f;
float screenWidth = 800.0f;

GLFWwindow *StartGLFW(){
    if (!glfwInit()){
        std::cerr << "Failed to initialize glfw" << std::endl;
        return nullptr;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "gravsim", NULL, NULL);

    return window;
}

class Object{
    public:
    std::vector<float> position;
    std::vector<float> velocity;
    float radius;

    Object(std::vector<float> position, std::vector<float> velocity, float radius = 15.0f){
        this->position = position;
        this->velocity = velocity;
        this->radius = radius;
    }

    void accelerate(float x, float y){
        this->velocity[0] += x;
        this->velocity[1] += y;
    }
    void updatePos(float dt){
        this->position[0] += this->velocity[0] * dt;
        this->position[1] += this->velocity[1] * dt;
    };
    void DrawCircle(int res = 100){
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(this->position[0], this->position[1]);
        for(int i = 0; i <= res; ++i){
            float angle = 2.0f * 3.141592653589 * (static_cast<float>(i) / res);
            float x = this->position[0] + cos(angle) * radius;
            float y = this->position[1] + sin(angle) * radius;
            glVertex2d(x, y);
        }
        glEnd();
    }
};

void resolveCollision(Object& a, Object& b, float restitution = 0.95f){
    float dx = b.position[0] - a.position[0];
    float dy = b.position[1] - a.position[1];
    float dist = std::sqrt((dx * dx) * (dy * dy));

    if (dist >= a.radius + b.radius) return;

    float nx = dx / dist;
    float ny = dy / dist;

    float dvn = (a.velocity[0] - b.velocity[0]) * nx + (a.velocity[1] - b.velocity[1]) * ny;

    if (dvn <= 0) return;

    float impulse = dvn * (1.0f + restitution) / 2.0f;

    a.velocity[0] -= impulse * nx;
    a.velocity[1] -= impulse * ny;
    b.velocity[0] += impulse * nx;
    b.velocity[1] += impulse * ny;

    float overlap = (a.radius + b.radius - dist) / 2.0f;

    a.position[0] -= overlap * nx;
    a.position[1] -= overlap * ny;
    b.position[0] += overlap * nx;
    b.position[1] += overlap * ny;
};

int main(){
    GLFWwindow *window = StartGLFW();
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glfwSwapInterval(1);

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float radius = 50.0f;
    int res = 100;

    std::vector<Object> objs = {
        Object(std::vector<float>{200.0f, 00.0f}, std::vector<float>{5.0f, 0.0f}),
        Object(std::vector<float>{600.0f,00.0f}, std::vector<float>{5.0f, 0.0f})
    };

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)){
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);

        for(auto& obj : objs){
            obj.accelerate(0.0f, 9.81f * 100.0f * dt);
            obj.updatePos(dt);
            obj.DrawCircle();

            if(obj.position[1] - obj.radius < 0){
                obj.position[1] = obj.radius;
                obj.velocity[1] *= -0.95f;
            } else if(obj.position[1] + obj.radius > screenHeight){
                obj.position[1] = screenHeight - obj.radius;
                obj.velocity[1] *= -0.95f;
            }
            if(obj.position[0] - obj.radius < 0){
                obj.position[0] = obj.radius;
                obj.velocity[0] *= -0.95f;
            } else if(obj.position[0] + obj.radius > screenWidth){
                obj.position[0] = screenWidth - obj.radius;
                obj.velocity[0] *= -0.95f;
            }
        }

        for (int i = 0; i < objs.size(); i++)
            for (int j = i + 1; j < objs.size(); j++)
                resolveCollision(objs[i], objs[j]);
                

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}