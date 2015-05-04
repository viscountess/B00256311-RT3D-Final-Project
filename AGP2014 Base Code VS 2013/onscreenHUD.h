#ifndef ONSCREEN_HUD_INCLUDE
#define ONSCREEN_HUD_INCLUDE

#include "TextBase.h"
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

class OnscreenHUD : public TextBase
{
public:

	//constructor
	OnscreenHUD();

	//deconstructor
	~OnscreenHUD();

	virtual void initialise();
	virtual void render();
	virtual void update();
	virtual GLuint textToTexture(const char * str);
};

#endif //ONSCREEN_HUD_INCLUDE