#pragma once

#include <glm/vec3.hpp>
#include <vector>
struct Particle
{
	bool fixed=false;
	double mass;
	glm::dvec3 positionNew, positionOld, velocity;
	
};

