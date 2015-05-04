#include "onscreenHUD.h"
#include "skybox.h"
#include "hobgoblin.h"
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

using namespace std;

//constructor
OnscreenHUD::OnscreenHUD()
{

}

//deconstructor
OnscreenHUD::~OnscreenHUD()
{

}

void OnscreenHUD::initialise()
{
	

	// set up TrueType / SDL_ttf
	if (TTF_Init() == -1)
		cout << "TTF failed to initialise." << endl;

	textFont = TTF_OpenFont("Royal Inferno.ttf", 40);
	if (textFont == NULL)
		cout << "Failed to open font." << endl;
	
	labels[0] = textToTexture(" Bunnies:  ");
	
	reset();
	
}

//Resetting all onscreen text ingame
void OnscreenHUD::reset()
{
	labels[1] = textToTexture(" 0 / 8 ");
	currCollectCount = 0;
}

void OnscreenHUD::render(std::stack<glm::mat4>& _Stack, Skybox *mySkybox)
{
	glm::mat4 projection(1.0);
	projection = glm::perspective(60.0f, 800.0f / 600.0f, 1.0f, 150.0f);
	rt3d::setUniformMatrix4fv(mySkybox->getShaderProgram(), "projection", glm::value_ptr(projection));
	////////////////////////////////////////////////////////////////////
	//This renders a HUD label
	////////////////////////////////////////////////////////////////////

	glUseProgram(mySkybox->getShaderProgram());//Use texture-only shader for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label
	glBindTexture(GL_TEXTURE_2D, labels[0]);
	_Stack.push(glm::mat4(1.0));
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(-0.4f, 0.8f, 0.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(0.40f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(mySkybox->getShaderProgram(), "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(mySkybox->getShaderProgram(), "modelview", glm::value_ptr(_Stack.top()));
	rt3d::drawIndexedMesh(mySkybox->getMeshObjects(), mySkybox->getMeshIndexCount(), GL_TRIANGLES);
	_Stack.pop();

	glBindTexture(GL_TEXTURE_2D, labels[1]);
	_Stack.push(glm::mat4(1.0));
	_Stack.top() = glm::translate(_Stack.top(), glm::vec3(0.2f, 0.8f, 0.0f));
	_Stack.top() = glm::scale(_Stack.top(), glm::vec3(0.35f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(mySkybox->getShaderProgram(), "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(mySkybox->getShaderProgram(), "modelview", glm::value_ptr(_Stack.top()));
	rt3d::drawIndexedMesh(mySkybox->getMeshObjects(), mySkybox->getMeshIndexCount(), GL_TRIANGLES);
	_Stack.pop();

}
void OnscreenHUD::update(Hobgoblin *myHobgoblin)
{
	if (myHobgoblin->getCollectCounter() != currCollectCount)
	{
		currCollectCount = myHobgoblin->getCollectCounter();
		char tmpLabel[8];
		_snprintf(tmpLabel, 8, " %i / 8", currCollectCount);
		labels[1] = textToTexture(tmpLabel);
	}
}
GLuint OnscreenHUD::textToTexture(const char * str)
{
	TTF_Font *font = textFont;
	SDL_Color colour = { 255, 255, 0 };
	SDL_Color bg = { 0, 0, 0 };

	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(font, str, colour);

	if (stringImage == NULL)
		//exitFatalError("String surface not created.");
		std::cout << "String surface not created." << std::endl;

	GLuint w = stringImage->w;
	GLuint h = stringImage->h;
	GLuint colours = stringImage->format->BytesPerPixel;

	GLuint format, internalFormat;
	if (colours == 4) {   // alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}
	else {             // no alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	internalFormat = (colours == 4) ? GL_RGBA : GL_RGB;

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0,
		format, GL_UNSIGNED_BYTE, stringImage->pixels);

	// SDL surface was used to generate the texture but is no longer
	// required. Release it to free memory
	SDL_FreeSurface(stringImage);
	return texture;
}