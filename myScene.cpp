////////////////////////////////////////////////////
// // Template code for  CS 174C
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <shared/defs.h>
#include "shared/opengl.h"
#include <string.h>
#include <util/util.h>
#include <GLModel/GLModel.h>
#include "anim.h"
#include "animTcl.h"
#include "myScene.h"

//Include the ParticleSystem class
#include "ParticleSystem.h"
#include "HermiteSpline.h"

#include "ParticleSimulator.h"
#include "ObjectPathSimulator.h"

//#include <util/jama/tnt_stopwatch.h>
//#include <util/jama/jama_lu.h>

// register a sample variable with the shell.
// Available types are:
// - TCL_LINK_INT 
// - TCL_LINK_FLOAT

int g_testVariable = 10;

SETVAR myScriptVariables[] = {
	"testVariable", TCL_LINK_INT, (char*)&g_testVariable,
	"",0,(char*)NULL
};


//---------------------------------------------------------------------------------
//			Hooks that are called at appropriate places within anim.cpp
//---------------------------------------------------------------------------------

// start or end interaction
void myMouse(int button, int state, int x, int y)
{

	// let the global resource manager know about the new state of the mouse 
	// button
	GlobalResourceManager::use()->setMouseButtonInfo(button, state);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button press event\n");

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button release event\n");
	}
}	// myMouse

// interaction (mouse motion)
void myMotion(int x, int y)
{

	GLMouseButtonInfo updatedMouseButtonInfo =
		GlobalResourceManager::use()->getMouseButtonInfo();

	if (updatedMouseButtonInfo.button == GLUT_LEFT_BUTTON)
	{
		animTcl::OutputMessage(
			"My mouse motion callback received a mousemotion event\n");
	}

}	// myMotion


void MakeScene(void)
{

	/*

	This is where you instantiate all objects, systems, and simulators and
	register them with the global resource manager
	*/

	/* SAMPLE SCENE */
	/* THE FOLLOWING IS DEDICATED TO ASSIGNMENT 2: PARTICLE SYSYEM*/
	boolean success;

	ParticleSystem* partSys = new ParticleSystem("partSys");
	success = GlobalResourceManager::use()->addSystem(partSys, true);
	assert(success);

	ParticleSimulator* partSim = new ParticleSimulator("partSim");
	success = GlobalResourceManager::use()->addSimulator(partSim, true);
	assert(success);


	// make sure it was registered successfully

	/**************************END OF ParticleSystem INSTANCE************************************/



}	// MakeScene

// OpenGL initialization
void myOpenGLInit(void)
{
	animTcl::OutputMessage("Initialization routine was called.");

}	// myOpenGLInit

void myIdleCB(void)
{

	return;

}	// myIdleCB

void myKey(unsigned char key, int x, int y)
{
	animTcl::OutputMessage("My key callback received a key press event\n");
	return;

}	// myKey



static int testPart1(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	animTcl::OutputMessage("Started part 1.");

	boolean success;

	HermiteSpline* hermite = new HermiteSpline("hermite");
	success = GlobalResourceManager::use()->addSystem(hermite, true);
	assert(success);
	return TCL_OK;
}	// test part1

static int testPart2(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	animTcl::OutputMessage("Started part 2.");

	/* STEP 1: Delete systems used for part 1*/

	/* STEP 2: Create an object and path systems*/
	boolean success;

	// Create an objectpath for the object as an instance of HermiteSpline class
	HermiteSpline* objectpath = new HermiteSpline("objectpath");
	success = GlobalResourceManager::use()->addSystem(objectpath, true);
	assert(success);

	ThreeDModel* threeDmodel = new ThreeDModel("threeDmodel");
	success = GlobalResourceManager::use()->addSystem(threeDmodel, true);
	assert(success);
	

	/* STEP 3: Create an object simulator*/
	ObjectPathSimulator* objectpathSimulator = new ObjectPathSimulator( "objectpathSimulator", objectpath, threeDmodel);

	success = GlobalResourceManager::use()->addSimulator( objectpathSimulator );

	// make sure it was registered successfully
	assert( success );

	return TCL_OK;
}	// test part2

void mySetScriptCommands(Tcl_Interp* interp)
{

	Tcl_CreateCommand(interp, "part1", testPart1, (ClientData) NULL,
		(Tcl_CmdDeleteProc*)NULL);
	Tcl_CreateCommand(interp, "part2", testPart2, (ClientData)NULL,
		(Tcl_CmdDeleteProc*)NULL);


}	// mySetScriptCommands