#include "bunnyPickup.h"

using namespace std;

//coonstructor
BunnyPickup::BunnyPickup(glm::vec3 _bunnyPos)
{
	bunnyPos = _bunnyPos;
}

//deconstructor
BunnyPickup::~BunnyPickup()
{

}

void BunnyPickup::initialise()
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
	rt3d::loadObj("bunny-5000.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	textures[0] = rt3d::loadBitmap("rock_texture.bmp");
	meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
}

void BunnyPickup::render(std::stack<glm::mat4>& _Stack)
{
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), bunnyPos);
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(5.0f, 0.1f, 5.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();
}

void BunnyPickup::update()
{

}