#ifndef HOBGOBLIN_INCLUDE
#define HOBGOBLIN_INCLUDE

#include "md2model.h"
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <SDL_ttf.h>

class Collectable;

class Hobgoblin
{
public:
	//constructor
	Hobgoblin();
	//deconstructor
	~Hobgoblin();
	void initialise();
	void render(std::stack<glm::mat4>& _stack);
	void update(void);
	glm::vec3 getPos() { return pos; }
	float getRotate() { return rotate; }

	
	//So that the collectable object can tell the player that it's in contact with it
	void setCurrCollectable(Collectable *_collectable){ currBunny = _collectable; };

private:
	// md2 stuff
	md2model tmpModel;
	int currentAnim;

	GLuint meshIndexCount = 0;
	GLuint md2VertCount = 0;
	GLuint meshObjects[2];

	GLuint textures[2];

	GLuint shaderProgram;

	rt3d::materialStruct material1;

	glm::vec3 pos;
	//GLfloat Zpos;

	float rotate;

	Collectable *currBunny;
};



#endif  //HOBGOBLIN_INCLUDE