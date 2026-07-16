#pragma once
#include <glad/glad.h>
#include "../bodies/gravsim_bodies.h"
#include <vector>
#include <cmath>

struct alignas(16) BodyGPU {
    double pos[2];
    double vel[2];
    double acc[2];
    double mass;
    double _pad;
};

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