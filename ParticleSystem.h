#pragma once

/*
			This is a Particle system.

*/

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>
#include "Particle.h"
#include "shared/opengl.h"
#include <vector>

class ParticleSystem : public BaseSystem
{
public:
	ParticleSystem(const std::string& name);
	// getters and setters
	void particlesInit(int numParticles);
	void setMass(int index, float mass);
	void setVelocity(int index, glm::vec3 velocity);
	void setPosition(int index, glm::vec3 position);
	float getMass(int index);
	glm::vec3 getVelocity(int index);
	glm::vec3 getPosition(int index);


	// The nescessary objects
	std::vector <Particle> particles;

	// Nescessary functions
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);
protected:

};