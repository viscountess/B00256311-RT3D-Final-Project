#ifndef TEXTBASE_INCLUDE
#define TEXTBASE_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

//This will be the abstract base class for all text displayed in the game

class TextBase
{
public:

	//constructor
	TextBase(){}

	//deconstructor
	virtual ~TextBase(){}

	virtual void initialise() = 0;
	virtual void render() = 0;
	virtual void update() = 0;
	virtual GLuint textToTexture(const char * str) = 0;


};
#endif //TEXTBASE