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
	int init(double time) 
	{
		return 0;
	}

	void updateSpringForces();

	// Integrators
	void integrateEuler(double dt);
	void integrateSymplectic(double accuracyStep);
	void integrateVerlet(double accuracyStep);

	int command(int argc, myCONST_SPEC char** argv);
	int step(double time);

	void display(GLenum mode = GL_RENDER);

protected:
	ParticleSystem* particleSys;
	std::vector <Spring> springs;
	double gravity=0;
	double kDrag=0;
	double ksGround=0;
	double kdGround=0;
	double accuracyStep=0;
	bool euler=false;
	bool symplectic=false;
	bool verlet=false;
};

