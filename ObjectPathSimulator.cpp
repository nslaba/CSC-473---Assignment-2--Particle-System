#include "ObjectPathSimulator.h"

ObjectPathSimulator::ObjectPathSimulator(const std::string& name, HermiteSpline* targetPath, ThreeDModel* targetObject) :
	BaseSimulator(name),
	splinePath(targetPath),
	m_threeDmodel(targetObject)
{
}	// ObjectPathSimulator

ObjectPathSimulator::~ObjectPathSimulator()
{
}	// ObjectPathSimulator::~ObjectPathSimulator


double ObjectPathSimulator::distance(double time)
{	
	float fullLength = splinePath->getFullLength();
	float vm = 10;

	double t0 = fullLength / (5 * vm);
	double t1 = 4 * t0;

	float distance = 0;
	double a0 = vm / t0;
	if (time < t0) {
		// b X h / 2
		
		distance = a0 * pow(time, 2) / 2.0;
	}
	else if (time >= t0 && time < (t0 + t1)) {
		// b X h / 2 + b X h
		distance = (fullLength / 10.0) + vm * (time - t0);
	}
	else if (time > (t0+t1) && time < (2*t0 + t1)){
		distance = fullLength - (a0 * (pow(2 * t0 + t1 - time, 2)) / 2);
	}
	else {
		distance = fullLength;
	}
	return distance;
}

int ObjectPathSimulator::step(double time)
{
	/* STEP 1: Get the correct distance travelled based on acceleration*/

	float distanceTravelled = distance(time);

	/* STEP 1. b) print the speed of the car to the console once per second*/
	if (timer >= 100) {

		animTcl::OutputMessage("at time: ");
		animTcl::OutputMessage(const_cast<char*>(std::to_string(time).c_str()));
		animTcl::OutputMessage("the speed is: ");
		animTcl::OutputMessage(const_cast<char*>(std::to_string(distanceTravelled / time).c_str()));
		
		timer = 0;
	}
	timer++;
	/* STEP 2: Calculate position based on P(u(s(t)))*/

	/* STEP 2. b) get parameter U based on distance travelled by threeDModel*/
	LookUpTableEntry tempEntry = LookUpTableEntry();
	tempEntry.arcLength = distanceTravelled;
	double u = splinePath->getU(tempEntry);

	/* STEP 2. c) get POSITION of m_threeDmodel based on U*/

	// create a temp point
	ControlPoint point = ControlPoint();
	point = splinePath->getPointAtU(u);
	m_threeDmodel->position = point.point;

	/* STEP 3 ACCOUNT FOR ROTATION*/

	/* STEP 3) a. update u,v,w*/
	m_threeDmodel->w = point.tangent;
	m_threeDmodel->u = glm::cross(point.tangent, point.secondTangent);
	m_threeDmodel->v = glm::cross(m_threeDmodel->w, m_threeDmodel->u);

	//normalize them
	m_threeDmodel->w = glm::normalize(m_threeDmodel->w);
	m_threeDmodel->u = glm::normalize(m_threeDmodel->u);
	m_threeDmodel->v = glm::normalize(m_threeDmodel->v);


	return 0;

}	// ObjectPathSimulator::step