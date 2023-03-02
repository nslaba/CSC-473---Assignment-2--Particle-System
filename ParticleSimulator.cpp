#include "ParticleSimulator.h"
#include "anim.h"
#include "animTcl.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

ParticleSimulator::ParticleSimulator(const std::string& name) :
	BaseSimulator(name)
	
{
}	// ParticleSimulator



//Integrators
void ParticleSimulator::integrateEuler(double dt)
{

	//for each particle
		
	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed)
		{
		//	update acceleration based on force over mass
			//particle.acceleration = force / particle.mass;
			particle.force = -kDrag * (particle.velocity) + particle.mass * gravity; // + external force + stiffness and damping for all spring pairs
			particle.acceleration = particle.force / particle.mass;
			animTcl::OutputMessage("gravity is: ");
			animTcl::OutputMessage("%f", gravity);
			animTcl::OutputMessage("kDrag is");
			animTcl::OutputMessage("%f", kDrag);
			animTcl::OutputMessage("old velocity is: ");
			animTcl::OutputMessage("%f", particle.velocity);
			animTcl::OutputMessage("acceleration is: ");
			animTcl::OutputMessage("%f", particle.acceleration);
		//		update new position based on old velocity * dt
			//using const velocity for now
			//particle.velocity = glm::dvec3(2.0, 2.0, 2.0);
			particle.velocity = particle.velocity + dt * particle.acceleration; //giving no reaction. velocity must be zero

			particle.positionNew = particle.positionOld + dt * particle.velocity;
			
		//		update new velocity based on old position + acceleration times dt
			particle.positionOld = particle.positionNew;
						
		}
	}
}
void ParticleSimulator::integrateSymplectic(double accuracyStep)
{

}
void ParticleSimulator::integrateVerlet(double accuracyStep)
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
		
		particleSys = dynamic_cast<ParticleSystem*>(GlobalResourceManager::use()->getSystem(sysName));

		if (particleSys == NULL) {
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
			// Save the spring Id in the existing particles' SpringID list
			particleSys->particles[spring.particleA].springIDs.push_back(springs.size());
			particleSys->particles[spring.particleB].springIDs.push_back(springs.size());

			spring.ks = atof(argv[3]);
			spring.kd = atof(argv[4]);
			spring.restlength = atof(argv[5]);
			
			// Account for if restlength is negative
			if (spring.restlength < 0)
			{
				spring.restlength = glm::distance(particleSys->particles[spring.particleA].positionOld, particleSys->particles[spring.particleB].positionOld);
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
			particleSys->particles[atoi(argv[1])].fixed = true;
		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "integration")==0)
	{ // simulator <sim_name> integration <euler|symplectic|verlet> <time step>
		if (argc == 3) {
			accuracyStep = atof(argv[2]);
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
			ksGround = atof(argv[1]);
			kdGround = atof(argv[2]);
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
			kDrag = atof(argv[1]);
		}
		else {
			animTcl::OutputMessage("wrong amount of arguments");
			return TCL_ERROR;
		}
	}
	glutPostRedisplay();
	return TCL_OK;
}

int ParticleSimulator::step(double time)
{
	// Compare anim dt (continuity) with given dt (accuracy)
	if (SIMULATION_TIME_STEP <= accuracyStep) 
	{
		
		accuracyStep = SIMULATION_TIME_STEP; 
		// no need of an extra forloop
		//animTcl::OutputMessage("calling euler 0");

		integrateEuler(accuracyStep);
		

		/* STEP 1:*/

		if (euler)
		{
			//animTcl::OutputMessage("calling euler 1");
			integrateEuler(accuracyStep);
		}
		else if (symplectic)
		{

		}
		else if (verlet)
		{

		}
	}
	else if (accuracyStep < SIMULATION_TIME_STEP) 
	{
		// loop through time t = accuracyStep += accuracyStep while accuracyStep < SIMULATION_TIME_STEP
		double dt = accuracyStep;
		
		while (dt <= SIMULATION_TIME_STEP)
		{
			//for each particle
		//		update acceleration based on force over mass
		//		update new velocity based on old position + acceleration times dt
		//		update new position based on old velocity * dt
		// draw

		/* STEP 1:*/
			integrateEuler(accuracyStep);
			if (euler)
			{
				//animTcl::OutputMessage("calling euler 2");
				integrateEuler(accuracyStep);
			}
			else if (symplectic)
			{

			}
			else if (verlet)
			{

			}
			//update dt
			dt += accuracyStep;
		}
		
		
	}
	// Then update old velocity to new after drawing.

	


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
		glVertex3dv(glm::value_ptr(particleSys->particles[spring.particleA].positionOld));
		glVertex3dv(glm::value_ptr(particleSys->particles[spring.particleB].positionOld));
		/*animTcl::OutputMessage("Is loop running?");
		animTcl::OutputMessage("%f, %f, %f", particles->particles[spring.particleA].positionOld.x, particles->particles[spring.particleA].positionOld.y, particles->particles[spring.particleA].positionOld.z);
		animTcl::OutputMessage("%f, %f, %f", particles->particles[spring.particleB].positionOld.x, particles->particles[spring.particleB].positionOld.y, particles->particles[spring.particleB].positionOld.z);
		animTcl::OutputMessage("indeces are");
		animTcl::OutputMessage("%d, %d", spring.particleA, spring.particleB);*/
	}
	glEnd();

	glColor3f(0.3, 0.7, 0.1);


	glPopMatrix();
	glPopAttrib();
}