#include "skybox.h"


using namespace std;
//std::stack<glm::mat4> mvStack;



Skybox::Skybox() //constructor
{

}

Skybox::~Skybox() //deconstructor
{

}

void Skybox::initialise()
{
	skyboxProgram = rt3d::initShaders("textured.vert", "textured.frag");

	skybox[0] = rt3d::loadBitmap("Town-skybox/skybox_hell_front.bmp");
	skybox[1] = rt3d::loadBitmap("Town-skybox/skybox_hell_back.bmp");
	skybox[2] = rt3d::loadBitmap("Town-skybox/skybox_hell_left.bmp");
	skybox[3] = rt3d::loadBitmap("Town-skybox/skybox_hell_right.bmp");
	//skybox[4] = loadBitmap("Town-skybox/Town_up.bmp");

	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
}

void Skybox::render(std::stack<glm::mat4>& _mvStack, glm::mat4& _projection)
{
	// draw a skybox
	glUseProgram(skyboxProgram);
	rt3d::setUniformMatrix4fv(skyboxProgram, "projection", glm::value_ptr(_projection));
	glDepthMask(GL_FALSE); // make sure depth test is off
	glm::mat3 mvRotOnlyMat3 = glm::mat3(_mvStack.top());
	_mvStack.push(glm::mat4(mvRotOnlyMat3));

	// front
	_mvStack.push(_mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	_mvStack.top() = glm::scale(_mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	_mvStack.top() = glm::translate(_mvStack.top(), glm::vec3(0.0f, 0.0f, -2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(_mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_mvStack.pop();

	// back
	_mvStack.push(_mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	_mvStack.top() = glm::scale(_mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	_mvStack.top() = glm::translate(_mvStack.top(), glm::vec3(0.0f, 0.0f, 2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(_mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_mvStack.pop();

	// left
	_mvStack.push(_mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	_mvStack.top() = glm::scale(_mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	_mvStack.top() = glm::translate(_mvStack.top(), glm::vec3(-2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(_mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_mvStack.pop();

	// right
	_mvStack.push(_mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	_mvStack.top() = glm::scale(_mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	_mvStack.top() = glm::translate(_mvStack.top(), glm::vec3(2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(_mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_mvStack.pop();


	_mvStack.pop();
}