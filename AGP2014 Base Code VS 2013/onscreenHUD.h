#ifndef ONSCREEN_HUD_INCLUDE
#define ONSCREEN_HUD_INCLUDE

#include "TextBase.h"

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
};

#endif //ONSCREEN_HUD_INCLUDE