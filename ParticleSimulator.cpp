#include "ParticleSimulator.h"
#include "anim.h"
#include "animTcl.h"
#include <glm/gtc/type_ptr.hpp>

ParticleSimulator::ParticleSimulator(const std::string& name) :
	BaseSimulator(name)
	
{
}	// ParticleSimulator



//Integrators
void ParticleSimulator::integrateEuler(double time)
{
	for (const auto& particle : particles->particles)
	{
		if (!particle.fixed)
		{
			//particle.position += timeStep * particle.velocity;
		}
	}
}
void ParticleSimulator::integrateSymplectic(double timeStep)
{

}
void ParticleSimulator::integrateVerlet(double timeStep)
{

}


int ParticleSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "link") == 0)
	{ //simulator <sim_name> link <sys name> <Number of Springs>
		int numSprings = atoi(argv[2]);
		std::string sysName = argv[1];
		

		// Retrieve the correct system & link it to the simulator
		
		particles = dynamic_cast<ParticleSystem*>(GlobalResourceManager::use()->getSystem(sysName));

		if (particles == NULL) {
			animTcl::OutputMessage("System doesn't exist");
			return TCL_ERROR;
		}
		
		/* reserve the right amount of memory for the vector of springs*/
		springs.reserve(numSprings);

		return TCL_OK;

	}
	else if (strcmp(argv[0], "spring") == 0)
	{ //simulator <sim_name> spring <index1> <index2> <ks> <kd> <restlength>
		if (argc == 6) {
			Spring spring = Spring();
			spring.particleA = atoi(argv[1]);
			spring.particleB = atoi(argv[2]);
			spring.ks = atof(argv[3]);
			spring.kd = atof(argv[4]);
			spring.restlength = atof(argv[5]);
			
			// Account for if restlength is negative
			if (spring.restlength < 0)
			{
				spring.restlength = glm::distance(particles->particles[spring.particleA].position, particles->particles[spring.particleB].position);
			}

			// add it to the array
			springs.push_back(spring);
		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
		
	}
	else if (strcmp(argv[0], "fix") == 0)
	{ // simulator <sim_name> fix <index>
		if (argc == 2) {
			particles->particles[atoi(argv[1])].fixed = true;
		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "integration")==0)
	{ // simulator <sim_name> integration <euler|symplectic|verlet> <time step>
		if (argc == 3) {
			timeStep = atof(argv[2]);
			if (strcmp(argv[1], "euler") == 0)
			{
				euler= true;
				symplectic, verlet = false;
			}
			else if (strcmp(argv[1], "symplectic") == 0)
			{
				symplectic= true;
				euler, verlet = false;
			}
			else if (strcmp(argv[1], "verlet") == 0)
			{
				verlet=true;
				euler, symplectic = false;
			}
		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "ground")==0)
	{ // simulator <sim_name> ground <ks> <kd>
		if (argc == 3)
		{

		}
		else
		{
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "gravity")==0)
	{ //simulator <sim_name> gravity <g>
		if (argc == 2)
		{
			gravity = atof(argv[1]);
		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "drag")==0)
	{ //simulator <sim_name> drag <kdrag>
		if (argc == 2)
		{

		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
}

int ParticleSimulator::step(double time)
{
	/* STEP 1: Get the correct distance travelled based on acceleration*/

	if (euler)
	{
		integrateEuler(time);
	}
	else if (symplectic)
	{

	}
	else if (verlet)
	{

	}


	return 0;

}	// ParticleSimulator::step


// To draw springs
void ParticleSimulator::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	
	glBegin(GL_LINES);
	for (const auto& spring: springs)
	{
		// Draw a line between two particles
		glVertex3dv(glm::value_ptr(particles->particles[spring.particleA].position));
		glVertex3dv(glm::value_ptr(particles->particles[spring.particleB].position));
		animTcl::OutputMessage("Is loop running?");
		animTcl::OutputMessage("%f, %f, %f", particles->particles[spring.particleA].position.x, particles->particles[spring.particleA].position.y, particles->particles[spring.particleA].position.z);
		animTcl::OutputMessage("%f, %f, %f", particles->particles[spring.particleB].position.x, particles->particles[spring.particleB].position.y, particles->particles[spring.particleB].position.z);
		animTcl::OutputMessage("indeces are");
		animTcl::OutputMessage("%d, %d", spring.particleA, spring.particleB);
	}
	glEnd();

	glColor3f(0.3, 0.7, 0.1);


	glPopMatrix();
	glPopAttrib();
}