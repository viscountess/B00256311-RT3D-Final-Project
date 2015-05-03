#include "movingPlatform.h"
#include <time.h>
#include "Utils.h"

using namespace std;

//constructor
MovingPlatform::MovingPlatform(glm::vec3 _startingPos, glm::vec3 _endPos)
{
	//variables for animating moving platform
	startingPos = _startingPos;
	currentPos = startingPos;
	endPos = _endPos;

	//direction of travel
	m_direction = 1;
}

//deconstructor
MovingPlatform::~MovingPlatform()
{

}

void MovingPlatform::initialise()
{
	material0 = {
		{ 0.0f, 0.0f, 0.5f, 1.0f }, // ambient
		{ 0.5f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
		2.0f  // shininess
	};

	waitTime = 0;

	shaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");
	rt3d::setMaterial(shaderProgram, material0);

	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	textures[0] = rt3d::loadBitmap("rock_texture.bmp");
	meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());

}

void MovingPlatform::render(std::stack<glm::mat4>& _Stack)
{

	//drawing the position of first moving platform
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), currentPos);
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(5.0f, 0.1f, 5.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();
}



void MovingPlatform::update(Hobgoblin *player)
{
	//collision detection for the player on moving platform
	glm::vec3 playerPos = player->getPos();
	if (playerPos.x > currentPos.x - 5 &&
		playerPos.x < currentPos.x + 5 &&
		playerPos.z > currentPos.z - 5 &&
		playerPos.z < currentPos.z + 5)
	{
		player->setCurrPlatform(this);
	}


	waitTime -= 0.1;

	if (waitTime > 0)
	{
		offsetThisTick = glm::vec3(0, 0, 0); //This is to make sure the player doesnt keep moving when the moving platform has stopped
		return;
	}
		
	glm::vec3 to;
	glm::vec3 from;

	if (m_direction == 1)
	{
		to = endPos;
		from = startingPos;
	}
	else
	{
		to = startingPos;
		from = endPos;
	}

	//stores the direction to target position
	glm::vec3 dir = to - from;
	//stores the distance between startingPos and endPos
	float distance = Utils::length(dir);

	//normalises the vector to make it a unit vector
	dir /= distance;

	offsetThisTick = dir;
	offsetThisTick *= 0.1; //speed
	currentPos += offsetThisTick;
	
	glm::vec3 dirToTarget = to - currentPos;
	dirToTarget /= Utils::length(dirToTarget);

	float dp = Utils::dot(dir, dirToTarget);
	if ( dp < 0)
	{
		currentPos = to;
		m_direction *= -1;
		waitTime = 20; //waiting time before the platform moves
	}



}