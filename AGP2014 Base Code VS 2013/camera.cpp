#include "camera.h"
#include "Utils.h"
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
	//Just putting in this call will reduce the number of lines of code
	reset();
}

//Resets all camera valaues 
void Camera::reset()
{
	eye = glm::vec3(0.0f, 1.0f, 0.0f);
	at = glm::vec3(0.0f, 1.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	extraRotation = 0;
}

//render the camera
void Camera::render(std::stack<glm::mat4>& _Stack)
{
	//at = moveForward(eye, r, 1.0f);
	_Stack.top() = glm::lookAt(eye, at, up);
}

//Passing in the hobgoblin position (_targetPos) so that
//the camera position can be updated
void Camera::update(glm::vec3 _targetPos, float _rotate, bool spinCam)
{
	//If the player has won, then spin the camera around the player
	if (spinCam)
		extraRotation += 0.5;

	eye = Utils::moveForward(_targetPos, _rotate + extraRotation, 5);
	eye.y += 2;
	at = _targetPos;

}