#ifndef MOVING_PLATFORM_INCLUDE
#define MOVING_PLATFORM_INCLUDE

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

class MovingPlatform
{
public:
	//constructor - passing in the start & end positions of each moving platform
	MovingPlatform(glm::vec3 _startingPos, glm::vec3 _endPos);

	//deconstructor
	~MovingPlatform();

	void initialise();

	void render(std::stack<glm::mat4>& _Stack);

	void update();

private:

	GLuint meshIndexCount = 0;
	GLuint md2VertCount = 0;
	GLuint meshObjects[2];

	GLuint shaderProgram;

	GLuint textures[2];

	rt3d::materialStruct material0;

	//member variables for animating moving platform
	glm::vec3 startingPos;
	glm::vec3 currentPos;
	glm::vec3 endPos;
	 
	//direction of travel
	int m_direction;

	float waitTime;
};
#endif //MOVING_PLATFORM_INCLUDE