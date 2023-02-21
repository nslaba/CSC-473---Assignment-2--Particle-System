#include "ParticleSimulator.h"
#include "anim.h"
#include "animTcl.h"

ParticleSimulator::ParticleSimulator(const std::string& name, ParticleSystem* target) :
	BaseSimulator(name),
	particleSys(target)
{
}	// ParticleSimulator

ParticleSimulator::~ParticleSimulator()
{
}	// ParticleSimulator::~ParticleSimulator

int ParticleSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "link") == 0)
	{ //simulator <sim_name> link <sys name> <Number of Springs>
		int numSprings = (int)argv[2];
		std::string sysName = argv[1];
		
		//register system
		bool success;
		ParticleSystem* name = new ParticleSystem(sysName);
		success = GlobalResourceManager::use()->addSystem(name, true);
		assert(success);

		//link this simulator to it


	}
}

int ParticleSimulator::step(double time)
{
	/* STEP 1: Get the correct distance travelled based on acceleration*/

	


	return 0;

}	// ParticleSimulator::step