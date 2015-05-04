#ifndef ONSCREEN_HUD_INCLUDE
#define ONSCREEN_HUD_INCLUDE

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

class OnscreenHUD : public TextBase
{
public:

	//constructor
	OnscreenHUD();

	//deconstructor
	~OnscreenHUD();

	virtual void initialise();
	virtual void render(std::stack<glm::mat4>& _Stack, Skybox *mySkybox);
	virtual void update(Hobgoblin *myHobgoblin);
	GLuint textToTexture(const char * str);

private:

	GLuint labels[5];
	TTF_Font * textFont;

	//number of collectables collected
	int currCollectCount;
};

#endif //ONSCREEN_HUD_INCLUDE