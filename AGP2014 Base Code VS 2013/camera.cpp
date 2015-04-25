#include "camera.h"

//constructor
Camera::Camera()
{

}

//deconstructor
Camera::~Camera()
{

}

//initialise the camera
void Camera::initialise()
{
	eye = glm::vec3(0.0f, 1.0f, 0.0f);
	at = glm::vec3(0.0f, 1.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

//render the camera
void Camera::render(std::stack<glm::mat4>& _Stack)
{
	//at = moveForward(eye, r, 1.0f);
	_Stack.top() = glm::lookAt(eye, at, up);
}

//Passing in the hobgoblin position (_targetPos) so that
//the camera position can be updated
void Camera::update(glm::vec3 _targetPos)
{
	eye = _targetPos - glm::vec3(0.0, -2, 5);
	at = _targetPos;

}