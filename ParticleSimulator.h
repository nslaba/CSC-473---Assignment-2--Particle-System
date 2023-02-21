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
class ParticleSimulator :
	public BaseSimulator
{
public:
	ParticleSimulator(const std::string& name, ParticleSystem* target);

	int command(int argc, myCONST_SPEC char** argv);
	int step(double time);

protected:
	ParticleSystem* particleSys;
};

