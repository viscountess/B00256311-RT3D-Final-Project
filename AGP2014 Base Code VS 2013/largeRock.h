#ifndef LARGE_ROCK_INCLUDE
#define LARGE_ROCK_INCLUDE

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

class LargeRock
{
public:
	//constructor
	LargeRock(glm::vec3 _rockPos, float _rockScale);

	//deconstructor
	~LargeRock();

	void initialise();

	void render(std::stack<glm::mat4>& _Stack);

	glm::vec3 getRockPos(){ return rockPos; }
	float getRockRadius(){ return rockRadius; }

private:

	GLuint meshIndexCount = 0;
	GLuint md2VertCount = 0;
	GLuint meshObjects[2];

	GLuint shaderProgram;

	GLuint textures[2];

	rt3d::materialStruct material0;

	float rockRadius;

	glm::vec3 rockPos; //rock position
	float rockScale; //rock scale
};
#endif //LARGE_ROCK_INCLUDE