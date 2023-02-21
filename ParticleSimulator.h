#pragma once

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "ParticleSystem.h"
#include <string>
#include "BaseSimulator.h"
#include "Spring.h"
#include <glm/geometric.hpp>
class ParticleSimulator :
	public BaseSimulator
{
public:
	ParticleSimulator(const std::string& name);

	//Integrators
	void integrateEuler(double time);
	void integrateSymplectic(double timeStep);
	void integrateVerlet(double timeStep);

	int command(int argc, myCONST_SPEC char** argv);
	int step(double time);

	void display(GLenum mode = GL_RENDER);

protected:
	ParticleSystem* particles;
	std::vector <Spring> springs;
	double gravity;
	double timeStep;
	bool euler;
	bool symplectic;
	bool verlet;
};

