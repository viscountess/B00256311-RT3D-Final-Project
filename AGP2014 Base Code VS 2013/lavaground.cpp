#include "lavaground.h"

using namespace std;

//constructor
LavaGround::LavaGround()
{

}

//deconstructor
LavaGround::~LavaGround()
{

}

//This function initialises the ground object
void LavaGround::initialise()
{
	material0 = {
		{ 1.0f, 1.0f, 0.0f, 1.0f }, // ambient
		{ 0.2f, 0.2f, 0.2f, 1.0f }, // diffuse
		{ 0.2f, 0.1f, 0.2f, 1.0f }, // specular
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
	//lava bitmap taken from www.dreamstime.com
	textures[0] = rt3d::loadBitmap("hot-lava-flow.bmp");
	meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
}

//This function renders the ground cube
void LavaGround::render(std::stack<glm::mat4>& _Stack)
{
	// draw a cube for ground plane
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(-10.0f, -0.5f, -10.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(60.0f, 0.1f, 100.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();
}