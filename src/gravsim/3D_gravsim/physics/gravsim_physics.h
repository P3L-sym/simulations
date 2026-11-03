#pragma once
#include "../bodies/gravsim_bodies.h"
#include <vector>

// CPU reference implementation. The running simulation uses the GPU version in
// compute/, but this is handy for checking the shader against.
Vec3 accelFrom(const Body& a, const Body& b);

void integrate(std::vector<Body>& bodies, double dt);
