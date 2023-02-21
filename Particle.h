#pragma once

#include <glm/vec3.hpp>
#include <vector>
struct Particle
{
	bool fixed;
	double mass;
	glm::dvec3 position, velocity;
	
};

