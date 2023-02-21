#include "ThreeDModel.h"

ThreeDModel::ThreeDModel(const std::string& name) :
	BaseSystem(name),
	position(0),
	distanceTravelled(0),
	velocity(10),
	u(0),
	v(0),
	w(0),
	transform(1.0f)
	
{
	m_model.ReadOBJ("data/porsche.obj");
	

}	// ThreeDModel





void ThreeDModel::reset(double time)
{

	position = { 0,0,0 };

}	// ThreeDModel::Reset



void ThreeDModel::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	// Initialize nescessary rotations
	glm::quat nintyDegreeAroundU = glm::quat(glm::vec3(u.x, u.y, (3.14 / 2.0) * u.z));
	glm::quat oneHeightyDegree = glm::quat(glm::vec3(0, (3.14), 0));
	
	// Update Transform matrix at display
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	transform = glm::scale(transform, glm::vec3(0.01f));
	
	// Initialize a rotation quaternion	
	glm::quat rotation = glm::quatLookAt(w, v);
	rotation *= nintyDegreeAroundU;
	rotation *= oneHeightyDegree;
	transform *= glm::toMat4(rotation);

	glMultMatrixf(&transform[0][0]);


	if (m_model.numvertices > 0)
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
	glPopAttrib();

}	// ThreeDModel::display