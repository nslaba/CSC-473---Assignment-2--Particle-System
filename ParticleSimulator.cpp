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



void ParticleSimulator::updateSpringForces()
{

	//first clear all the forces the particles contain
	for (auto& particle : particleSys->particles)
	{
		particle.force = glm::dvec3{ 0.0 };
	}
	// update spring force for all connected particles
	for (auto& spring : springs)
	{
		// save particle A and particle B for clarity of the calculations
		Particle particleA = particleSys->particles[spring.particleA];
		Particle& particleB = particleSys->particles[spring.particleB];


		// STIFFNESS for particle A
		particleA.force = spring.ks
			* (spring.restlength - glm::length(particleA.positionCur - particleB.positionCur))
			* glm::normalize(particleA.positionCur - particleB.positionCur);
		

		// DAMPING for particle A
		particleA.force += -spring.kd
			* glm::dot((particleA.velocity - particleB.velocity), glm::normalize(particleA.positionCur - particleB.positionCur))
			* glm::normalize(particleA.positionCur - particleB.positionCur);
	

		// STIFFNESS & DAMPING for particle B
		particleB.force += -particleA.force;

		particleSys->particles[spring.particleA].force += particleA.force;
	}
}

//Integrators
void ParticleSimulator::integrateEuler(double dt)
{
	// Update Spring forces
	updateSpringForces();
	//for each particle
		
	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed)
		{
		//	update acceleration based on force over mass
			//add the remaining nescessary forces to each particle
			particle.force += -kDrag * (particle.velocity) + particle.mass * gravity; 
			// ground collision detection
				// assuming that ground is zero -> P = (0, 0, 0)
			if (particle.positionCur[1] <= 0)
			{
				glm::dvec3 groundNormal = { 0.0f,1.0f,0.0f };
				// handle resolution
				particle.force += -ksGround * (particle.positionCur[1]) * groundNormal - kdGround * particle.velocity[1] * groundNormal;

			}

			particle.acceleration = particle.force / particle.mass;
			animTcl::OutputMessage("acceleration is %f\n", length(particle.acceleration));
			//particle.acceleration = glm::dvec3(3, 3, 3);
		//	update new position based on old velocity * dt
			particle.positionNew = particle.positionCur + dt * particle.velocity;

			particle.velocity = particle.velocity + dt * particle.acceleration; 
		
						
		}
	}
	//update positions for particles
	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed) {
			particle.positionPrev = particle.positionCur;
			particle.positionCur = particle.positionNew;
		}
	}
}
void ParticleSimulator::integrateSymplectic(double dt)
{
	// Update Spring forces
	updateSpringForces();
	//for each particle

	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed)
		{
			//	update acceleration based on force over mass
				//add the remaining nescessary forces to each particle
			particle.force += -kDrag * (particle.velocity) + particle.mass * gravity;
			
			// ground collision detection
			// assuming that ground is zero -> P = (0, 0, 0)
			if (particle.positionCur[1] <= 0)
			{
				glm::dvec3 groundNormal = { 0.0f,1.0f,0.0f };
				// handle resolution
				particle.force += -ksGround * (particle.positionCur[1]) * groundNormal - kdGround * particle.velocity[1] * groundNormal;

			}
			particle.acceleration = particle.force / particle.mass;
			animTcl::OutputMessage("acceleration is %f\n", length(particle.acceleration));
			//	update new position based on new velocity * dt

			particle.velocity = particle.velocity + dt * particle.acceleration;


			particle.positionNew = particle.positionCur + dt * particle.velocity;


		}
	}
	//update positions for particles
	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed) {
			particle.positionPrev = particle.positionCur;
			particle.positionCur = particle.positionNew;
		}
	}
}
void ParticleSimulator::integrateVerlet(double dt)
{

	// Update Spring forces
	updateSpringForces();
	//for each particle

	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed)
		{
			//	update acceleration based on force over mass
				//add the remaining nescessary forces to each particle
			particle.force += -kDrag * (particle.velocity) + glm::dvec3(0.0, particle.mass * gravity, 0.0);
			// ground collision detection
			// assuming that ground is zero -> P = (0, 0, 0)
			if (particle.positionCur[1] <= 0)
			{
				glm::dvec3 groundNormal = { 0.0f,1.0f,0.0f };
				// handle resolution
				particle.force += -ksGround * (particle.positionCur[1]) * groundNormal - kdGround * particle.velocity[1] * groundNormal;

			}
			particle.acceleration = particle.force / particle.mass;
			
			//	update new position 

			particle.positionNew = 2.0*particle.positionCur - particle.positionPrev + particle.acceleration * pow(dt,2);
			animTcl::OutputMessage("position prev is: %f\n", length(particle.positionPrev));

			particle.velocity =  ((particle.positionNew - particle.positionPrev)/ 2.0*dt) ;

		}
	}
	//update positions for particles
	for (auto& particle : particleSys->particles)
	{
		if (!particle.fixed) {
			particle.positionPrev = particle.positionCur;
			particle.positionCur = particle.positionNew;
		}
	}
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
			spring.ks = atof(argv[3]);
			spring.kd = atof(argv[4]);
			spring.restlength = atof(argv[5]);
			
			// Account for if restlength is negative
			if (spring.restlength < 0)
			{
				spring.restlength = glm::distance(particleSys->particles[spring.particleA].positionCur, particleSys->particles[spring.particleB].positionCur);
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

				// update the initial positionPrev for all particles
				for (auto& particle : particleSys->particles)
				{
					particle.positionPrev = particle.positionCur - particle.velocity * accuracyStep;
				}
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
	

	

	// Keep track of the remaining difference between simulation time and dt. 
	static float remainder = SIMULATION_TIME_STEP;

	while (remainder >= accuracyStep)
	{
		// update the system 
		if (euler)
		{
			integrateEuler(accuracyStep);
		}
		else if (symplectic)
		{
			integrateSymplectic(accuracyStep);
		}
		else if (verlet)
		{
			integrateVerlet(accuracyStep);
		}

		//update the remainder
		remainder -= accuracyStep;

	}

	remainder += SIMULATION_TIME_STEP;


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
		glVertex3dv(glm::value_ptr(particleSys->particles[spring.particleA].positionCur));
		glVertex3dv(glm::value_ptr(particleSys->particles[spring.particleB].positionCur));
		/*animTcl::OutputMessage("Is loop running?");
		animTcl::OutputMessage("%f, %f, %f", particles->particles[spring.particleA].positionCur.x, particles->particles[spring.particleA].positionCur.y, particles->particles[spring.particleA].positionCur.z);
		animTcl::OutputMessage("%f, %f, %f", particles->particles[spring.particleB].positionCur.x, particles->particles[spring.particleB].positionCur.y, particles->particles[spring.particleB].positionCur.z);
		animTcl::OutputMessage("indeces are");
		animTcl::OutputMessage("%d, %d", spring.particleA, spring.particleB);*/
	}
	glEnd();

	glColor3f(0.3, 0.7, 0.1);


	glPopMatrix();
	glPopAttrib();
}