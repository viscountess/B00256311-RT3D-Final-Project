#ifndef LAVAGROUND_INCLUDE
#define LAVAGROUND_INCLUDE

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

class LavaGround
{
public:

	//constructor
	LavaGround();

	//deconstructor
	~LavaGround();

	void initialise();
	void render(std::stack<glm::mat4>& _Stack);

private:

	GLuint meshIndexCount = 0;
	GLuint md2VertCount = 0;
	GLuint meshObjects[2];

	GLuint shaderProgram;

	GLuint textures[2];

	rt3d::materialStruct material0;
};

#endif //LAVAGROUND_INCLUDE