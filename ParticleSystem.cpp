#include "ParticleSystem.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include <algorithm>

ParticleSystem::ParticleSystem(const std::string& name):
	BaseSystem(name)
{
	
}

// getters and setters
void ParticleSystem::particlesInit(int numParticles)
{
	for (int i = 0; i < numParticles; i++)
	{
		Particle particle = Particle();
		particle.positionCur = glm::vec3{ 0.0f };
		particles.push_back(particle);
	}
}


void ParticleSystem::setMass(int index, float mass)
{
	particles[index].mass = mass;
}


void ParticleSystem::setVelocity(int index, glm::vec3 velocity)
{
	particles[index].velocity = velocity;
}


void ParticleSystem::setPosition(int index, glm::vec3 position)
{
	particles[index].positionCur = position;
}


float ParticleSystem::getMass(int index)
{
	return particles[index].mass;
}

glm::vec3 ParticleSystem::getVelocity(int index)
{
	return particles[index].velocity;
}

glm::vec3 ParticleSystem::getPosition(int index)
{
	return particles[index].positionCur;
}


// Nescessary functions


int ParticleSystem::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "dim") == 0)
	{
		int numParticles = atoi(argv[1]);
		particlesInit(numParticles);
		return TCL_OK;

	}
	else if (strcmp(argv[0], "particle") == 0)

		
	{ //system <sys_name> particle <index> <mass> <x y z vx vy vz>
		if (argc == 9) {
			Particle particle = Particle();
			int index = atoi(argv[1]);
			// mass
			particle.mass = std::stof(argv[2]);
			// position
			particle.positionCur = glm::vec3{ std::stof(argv[3]), std::stof(argv[4]), std::stof(argv[5]) };
			//to prevent error make new and old positions the same at the beginning
			particle.positionNew = particle.positionCur;
			
			// velocity
			particle.velocity = glm::vec3{ std::stof(argv[6]), std::stof(argv[7]), std::stof(argv[8]) };
			
			particles[index]=particle;
		}
		else 
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "all_velocities") == 0)
	{ // system <sys_name> all_velocities  <vx vy vz>
		if (argc == 4)
		{
			glm::vec3 velocity{ std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]) };
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i].velocity = velocity;
			}
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}

	glutPostRedisplay();
	return TCL_OK;

}	// ParticleSystem::command


// System draws the particles
void ParticleSystem::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	for (int i = 0; i < particles.size(); i++)
	{
		glPushMatrix();

		glTranslated(particles[i].positionCur.x, particles[i].positionCur.y, particles[i].positionCur.z);
		// Draw a particle at its given location
		glutSolidSphere(0.1, 5, 5);

		glPopMatrix();
		//animTcl::OutputMessage("old position of particles being drawn in system: ");
		//animTcl::OutputMessage("%f, %f, %f", particles[i].positionCur.x, particles[i].positionCur.y, particles[i].positionCur.z);
	}
	

	glColor3f(0.3, 0.7, 0.1);


	glPopMatrix();
	glPopAttrib();
}