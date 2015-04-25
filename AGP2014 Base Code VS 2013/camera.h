#ifndef CAMERA_INCLUDE
#define CAMERA_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

class Camera
{
public:
	//constructor
	Camera();
	//deconstructor
	~Camera();
	void initialise();
	void render(std::stack<glm::mat4>& _Stack);
	void update(glm::vec3 _targetPos, float _rotate);


private:
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
};
#endif //CAMERA_INCLUDE