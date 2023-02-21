#pragma once

/*
			This is a HermiteSpline system.
			
*/

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>
#include "ControlPoint.h"
#include "LookUpTableEntry.h"
#include "shared/opengl.h"
#include <vector>

#define MAX_KNOTS 40



// HermiteSpline system
class HermiteSpline : public BaseSystem
{

public:
	HermiteSpline(const std::string& name);


	 /*Setters needed for simulation*/
	// returns the full length --> to know how long the max length is
	float getFullLength();
	
	// returns parameter U based on arclength S
	float getU(LookUpTableEntry);
	// returns a Control point at a certain parameter u of the spline
	ControlPoint getPointAtU(float u);


	// Nescessary functions
	void display(GLenum mode = GL_RENDER);
	int command(int argc, myCONST_SPEC char** argv);
	float getArcLength(float t);
	void load(std::string);
	ControlPoint getNext(ControlPoint, ControlPoint, double);
	void updateLookUpTable();

	// Nescessary variables
	std::vector <LookUpTableEntry> lookUpTable;
	/* Define an array of ControlPoint structs with maximum of 40 control points*/
	ControlPoint controlPoints[MAX_KNOTS];
	//keeps track of the knots set by the user
	int numKnots = 0;


protected:
	
		

	

};
