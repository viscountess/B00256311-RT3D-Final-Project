#include "hobgoblin.h"
#include <SDL_ttf.h>
#include "Utils.h"
using namespace std;



//constructor
Hobgoblin::Hobgoblin()
{
	int currentAnim = 0;
}

//deconstructor
Hobgoblin::~Hobgoblin()
{

}

void Hobgoblin::initialise()
{
	textures[1] = rt3d::loadBitmap("hobgoblin2.bmp");
	meshObjects[1] = tmpModel.ReadMD2Model("tris.MD2");
	md2VertCount = tmpModel.getVertDataCount();

	material1 = {
		{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
		1.0f  // shininess
	};

	shaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");
	//rt3d::setLight(shaderProgram, light0);

	pos.x = 20.0f;
	pos.y = 1.2f;
	pos.z = -60.0f;
	rotate = 0;
}

void Hobgoblin::render(std::stack<glm::mat4>& _Stack)
{
	GLfloat scale(1.0f);

	// Animate the md2 model, and update the mesh with new vertex data
	tmpModel.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObjects[1], RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	// draw the hobgoblin
	glCullFace(GL_FRONT); // md2 faces are defined clockwise, so cull front face
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	rt3d::materialStruct tmpMaterial = material1;
	rt3d::setMaterial(shaderProgram, tmpMaterial);
	_Stack.push(_Stack.top());
	_Stack.top() = glm::translate(_Stack.top(), pos);
	_Stack.top() = glm::rotate(_Stack.top(), rotate + 90.f, glm::vec3(0.0f, -1.0f, 0.0f));
	_Stack.top() = glm::rotate(_Stack.top(), 90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
	
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(scale*0.05, scale*0.05, scale*0.05));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(_Stack.top()));
	rt3d::drawMesh(meshObjects[1], md2VertCount, GL_TRIANGLES);
	_Stack.pop();
	glCullFace(GL_BACK);
}


void Hobgoblin::update(void)
{
	currentAnim = 0;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]){
		//eye = moveForward(eye, r, 0.1f);
		//pos.z += 0.1f;
		pos = Utils::moveForward(pos, rotate, -0.1f);
		currentAnim = 1;

	}
	if (keys[SDL_SCANCODE_S]) //eye = moveForward(eye, r, -0.1f);
	{
		//pos.z -= 0.1f;
		pos = Utils::moveForward(pos, rotate, 0.1f);
		currentAnim = 1;
	}

	//strafe left
	if (keys[SDL_SCANCODE_A])
	{
		currentAnim = 1;
		pos = Utils::moveRight(pos, rotate, 0.1f);
	}

	//strafe right
	if (keys[SDL_SCANCODE_D])
	{
		currentAnim = 1;
		pos = Utils::moveRight(pos, rotate, -0.1f);
	}

	//look left
	if (keys[SDL_SCANCODE_Q]) //eye = moveRight(eye, r, -0.1f);
	{
		rotate -= 0.4;
	}

	//look right
	if (keys[SDL_SCANCODE_E]) //eye = moveRight(eye, r, 0.1f);
	{
		rotate += 0.4;
	}

	

	if (keys[SDL_SCANCODE_Z]) {
		if (--currentAnim < 0) currentAnim = 19;
		cout << "Current animation: " << currentAnim << endl;
	}
	if (keys[SDL_SCANCODE_X]) {
		if (++currentAnim >= 20) currentAnim = 0;
		cout << "Current animation: " << currentAnim << endl;
	}
}