#ifndef ONSCREEN_WIN_HUD_INCLUDE
#define ONSCREEN_WIN_HUD_INCLUDE

#include "TextBase.h"
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <SDL_ttf.h>

//Forward declaration of classes used
class Skybox;
class Hobgoblin;


class OnscreenWinHUD : public TextBase
{
public:
	//constructor
	OnscreenWinHUD();

	//deconstructor
	~OnscreenWinHUD();

	void initialise();
	void render(std::stack<glm::mat4>& _Stack, Skybox *mySkybox);
	void update(Hobgoblin *myHobgoblin);
	GLuint textToTexture(const char * str);
	void reset();

private:

	GLuint labels[5];
	TTF_Font * textFont;

	//member variable for if winning screen is active
	bool isActive;
};

#endif //ONSCREEN_WIN_HUD_INCLUDE