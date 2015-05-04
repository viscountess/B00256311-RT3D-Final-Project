#ifndef COLLECTABLE_INCLUDE
#define COLLECTABLE_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "hobgoblin.h"

//This will be the abstract base class for all collectables
//Using inheritance so if time permitted I can add in more collectables in the game

class Collectable
{
public:

	//constructor
	Collectable(){}

	//deconstructor
	virtual ~Collectable(){}

	virtual void initialise() = 0;
	virtual void render(std::stack<glm::mat4>& _Stack) = 0;
	virtual void update(Hobgoblin *player) = 0;
	virtual void pickUp() = 0;
	virtual void reset() = 0;
	
};

#endif //COLLECTABLE_INCLUDE