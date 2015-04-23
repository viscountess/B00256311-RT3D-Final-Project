#ifndef SKYBOX_INCLUDE
#define SKYBOX_INCLUDE

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>


class Skybox{
public:
	Skybox(); //constructor
	~Skybox(); //deconstructor
	void initialise();
	void render(std::stack<glm::mat4>& _stack, glm::mat4& _projection);
private:

	GLuint skyboxProgram;
	GLuint skybox[5];

	GLuint meshIndexCount = 0;
	GLuint md2VertCount = 0;
	GLuint meshObjects[2];
};


#endif //SKYBOX_INCLUDE