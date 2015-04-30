#ifndef BUNNY_PICKUP_INCLUDE
#define BUNNY_PICKUP_INCLUDE

#include "collectable.h"
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>


class BunnyPickup : public Collectable
{
public:

	//coonstructor
	BunnyPickup(glm::vec3 _bunnyPos);

	//deconstructor
	~BunnyPickup();

	virtual void initialise();
	virtual void render(std::stack<glm::mat4>& _Stack);
	virtual void update(Hobgoblin *player);
	virtual void pickUp() { isPickedUp = true; }

private:

	GLuint meshIndexCount = 0;
	GLuint md2VertCount = 0;
	GLuint meshObjects[2];

	GLuint shaderProgram;

	GLuint textures[2];

	rt3d::materialStruct material0;

	//member variable for position of bunny
	glm::vec3 bunnyPos;

	float rotate;
	bool isPickedUp;
};

#endif //BUNNY_PICKUP_INCLUDE