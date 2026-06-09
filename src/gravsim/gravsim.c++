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
    };
    void updatePos(){
        this->position[0] += this->velocity[0];
        this->position[1] += this->velocity[1];
    };
    void DrawCircle(int res = 50){
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

int main(){
    GLFWwindow *window = StartGLFW();
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float radius = 50.0f;
    int res = 100;

    std::vector<Object> objs = {
        Object(std::vector<float>{200.0f, 00.0f}, std::vector<float>{5.0f, 0.0f}),
        Object(std::vector<float>{700.0f,00.0f}, std::vector<float>{5.0f, 0.0f})
    };

    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        for(auto& obj : objs){
            obj.accelerate(0.0f, 9.81);
            obj.updatePos();
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}