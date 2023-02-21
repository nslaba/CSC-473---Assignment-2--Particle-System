#pragma once

#include <glm/vec3.hpp>
#include <vector>
struct Particle
{
	bool fixed;
	float mass;
	glm::vec3 position, velocity;
	std::vector<int> connectionIDs;
};

