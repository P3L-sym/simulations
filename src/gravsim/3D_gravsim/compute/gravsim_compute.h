#pragma once
#include <glad/glad.h>
#include "../bodies/gravsim_bodies.h"
#include <vector>
#include <cmath>

// Ten plain doubles = 80 bytes, 8-byte aligned. This matches the std430 layout
// of the shader-side struct exactly. (dvec3 would be padded to 32 bytes in
// std430, which is why the shader uses scalar doubles instead.)
struct BodyGPU {
    double pos[3];
    double vel[3];
    double acc[3];
    double mass;
};
static_assert(sizeof(BodyGPU) == 80, "BodyGPU must match the std430 layout in the shader");

class GravCompute {
public:
    ~GravCompute();

    void init(int numBodies);
    void upload(const std::vector<Body>& bodies);
    void download(std::vector<Body>& bodies) const;

    void integrateStep(double dt);

private:
    GLuint m_ssbo = 0;
    GLuint m_kickPosPrg = 0;
    GLuint m_kickVelPrg = 0;
    int m_n = 0;

    GLint m_locPosN = -1, m_locPosDt = -1;
    GLint m_locVelN = -1, m_locVelDt = -1, m_locVelG = -1, m_locVelSoft = -1;

    void kickPos(double c_dt);
    void kickVel(double d_dt);

    static GLuint makeComputeProgram(const char* src);
};
