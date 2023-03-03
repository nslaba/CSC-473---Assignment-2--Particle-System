#pragma once

#include <glm/vec3.hpp>
#include <vector>
struct Particle
{
	bool fixed=false;
	double mass;
	glm::dvec3 positionNew, positionCur, positionPrev, velocity = { 0,0,0 }, acceleration, force;
	
};

