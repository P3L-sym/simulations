#pragma once
#include "../bodies/gravsim_bodies.h"
#include <vector>

Vec2 accelFrom(const Body& a, const Body& b);

void integrate(std::vector<Body>& bodies, double dt);