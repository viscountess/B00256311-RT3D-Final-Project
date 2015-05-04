#include "ground.h"
#include "hobgoblin.h"

using namespace std;

//constructor
Ground::Ground()
{

}

//deconstructor
Ground::~Ground()
{

}

//This function initialises the ground object
void Ground::initialise()
{
	material0 = {
		{ 0.0f, 0.0f, 0.0f, 1.0f }, // ambient
		{ 0.5f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
		2.0f  // shininess
	};

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

//This function renders the ground cube
void Ground::render(std::stack<glm::mat4>& _Stack)
{
	// draw a cube for main ground plane
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(20.0f, -0.1f, -60.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(20.0f, 0.1f, 20.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();


	//Creating the first static ground
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(20.0f, -0.1f, 0.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(10.0f, 0.1f, 10.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();

	//Creating the second static ground
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(-40.0f, -0.1f, -80.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(10.0f, 0.1f, 10.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();

	//Creating the third static ground
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(-50.0f, -0.1f, 65.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(10.0f, 0.1f, 10.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();

	//Creating the fourth static ground
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(30.0f, -0.1f, 70.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(10.0f, 0.1f, 10.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();
}

void Ground::update(Hobgoblin *myHobgoblin)
{
	myHobgoblin->setOnSolidGround(false);

	//Give extra buffer for collision detection
	float extraBuffer = 0.2;

	//collision detection for the player on main solid platform
	glm::vec3 playerPos = myHobgoblin->getPos();
	if (playerPos.x > (0 - extraBuffer) &&
		playerPos.x < (40 + extraBuffer) &&
		playerPos.z > (-80 - extraBuffer) &&
		playerPos.z < (-40 + extraBuffer))
	{
		myHobgoblin->setOnSolidGround(true);
		return;
	}

	//collision detection for the player on first small solid platform
	if (playerPos.x > (10 - extraBuffer) &&
		playerPos.x < (30 + extraBuffer) &&
		playerPos.z > (-10 - extraBuffer) &&
		playerPos.z < (10 + extraBuffer))
	{
		myHobgoblin->setOnSolidGround(true);
		return;
	}

	//collision detection for the player on second small solid platform
	if (playerPos.x >(-50 - extraBuffer) &&
		playerPos.x < (-30 + extraBuffer) &&
		playerPos.z >(-90 - extraBuffer) &&
		playerPos.z < (-70 + extraBuffer))
	{
		myHobgoblin->setOnSolidGround(true);
		return;
	}

	//collision detection for the player on third small solid platform
	if (playerPos.x >(-60 - extraBuffer) &&
		playerPos.x < (-40 + extraBuffer) &&
		playerPos.z >(55 - extraBuffer) &&
		playerPos.z < (75 + extraBuffer))
	{
		myHobgoblin->setOnSolidGround(true);
		return;
	}

	//collision detection for the player on fourth small solid platform
	if (playerPos.x >(20 - extraBuffer) &&
		playerPos.x < (40 + extraBuffer) &&
		playerPos.z >(60 - extraBuffer) &&
		playerPos.z < (80 + extraBuffer))
	{
		myHobgoblin->setOnSolidGround(true);
		return;
	}
}