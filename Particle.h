#pragma once

#include <glm/vec3.hpp>
#include <vector>
struct Particle
{
	bool fixed=false;
	double mass;
	std::vector<int> springIDs;
	glm::dvec3 positionNew, positionOld, velocity = { 0,0,0 }, acceleration, force;
	
};

