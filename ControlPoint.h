#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
struct ControlPoint
{
	bool empty = true;
	glm::vec3 point;
	glm::vec3 tangent;
	glm::vec3 secondTangent;
};
