#ifndef TEXTBASE_INCLUDE
#define TEXTBASE_INCLUDE

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

//Forward declaration
class Skybox;
class Hobgoblin;

//This will be the abstract base class for all text displayed in the game

class TextBase
{
public:

	//constructor
	TextBase(){}

	//deconstructor
	virtual ~TextBase(){}

	virtual void initialise() = 0;
	virtual void render(std::stack<glm::mat4>& _Stack, Skybox *mySkybox) = 0;
	virtual void update(Hobgoblin *myHobgoblin) = 0;
	virtual GLuint textToTexture(const char * str) = 0;
	virtual void reset() = 0;


};
#endif //TEXTBASE_INCLUDE