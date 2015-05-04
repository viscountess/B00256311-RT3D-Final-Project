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
#include "largeRock.h"
#include "bass.h"

//forward declarations of classes
class Collectable;
class MovingPlatform;


class Hobgoblin
{
public:
	//constructor
	Hobgoblin();
	//deconstructor
	~Hobgoblin();
	void initialise();
	void render(std::stack<glm::mat4>& _stack);
	void update(LargeRock **_rocks, int numberOfRocks);
	glm::vec3 getPos() { return pos; }
	float getRotate() { return rotate; }
	int getCollectCounter() { return collectCounter; }
	bool hasWon(){ return collectCounter == maxNumOfCollectables; }
	bool hasDied() { return isDead; }
	void reset();
	void setOnSolidGround(bool onGround) { isOnSolidGround = onGround; }
	HSAMPLE loadSound(char * filename);

	
	//So that the collectable object can tell the player that it's in contact with it
	void setCurrCollectable(Collectable *_collectable){ currBunny = _collectable; }

	//This function will tell the program that the player is in contact with the moving platform
	void setCurrPlatform(MovingPlatform *_currPlatform){ currPlatform = _currPlatform; }

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

	MovingPlatform *currPlatform;

	//member variable for collectable counter
	int collectCounter;

	//member variable for maximum number of collectables
	int maxNumOfCollectables;

	bool isOnSolidGround;

	bool isDead;

	HSAMPLE pickupSound;
	HSAMPLE deathSound;
};



#endif  //HOBGOBLIN_INCLUDE