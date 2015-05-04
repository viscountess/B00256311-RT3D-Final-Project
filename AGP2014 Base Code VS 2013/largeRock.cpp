#include "largeRock.h"

using namespace std;

//constructor
LargeRock::LargeRock(glm::vec3 _rockPos, float _rockScale, float _rotate)
{
	rockPos = _rockPos;
	rockScale = _rockScale;
	rotate = _rotate;
}

//deconstructor
LargeRock::~LargeRock()
{

}

void LargeRock::initialise()
{
	rotate = 0;

	material0 = {
		{ 1.0f, 1.0f, 0.0f, 1.0f }, // ambient
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
	rt3d::loadObj("rock1.1.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	textures[0] = rt3d::loadBitmap("stone_texture.bmp");
	meshObjects[0] = rt3d::createMesh(verts.size()/3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());

	rockRadius = 0;

	for (int i = 0; i < verts.size(); i+=3)
	{
		GLfloat x = verts[i];
		GLfloat z = verts[i + 2];

		//set the radius of the rock to absolute length of vertice from 0 
		//if its greater than the current radius
		if (abs(x) > rockRadius)
			rockRadius = abs(x);

		if (abs(z) > rockRadius)
			rockRadius = abs(z);

	}

	//adjust the rock radius to the scale of the rock
	rockRadius *= rockScale;

	//debug purposes
	cout << "Rock Radius" << rockRadius << endl;
}

void LargeRock::render(std::stack<glm::mat4>& _Stack)
{
	//draw rock
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), rockPos);
	_Stack.top() = glm::rotate(_Stack.top(), rotate, glm::vec3(0.0f, -1.0f, 0.0f));
	//_Stack.top() = glm::rotate(_Stack.top(), 90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));

	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(rockScale, rockScale, rockScale));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	_Stack.pop();
	
}