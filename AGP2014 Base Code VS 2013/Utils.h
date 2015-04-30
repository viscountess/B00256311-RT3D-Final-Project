#ifndef UTILS_INCLUDE
#define UTILS_INCLUDE

#define DEG_TO_RADIAN 0.017453293

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//to define the moveForward & moveRight functions in one place
//so I can use them in multiple classes
class Utils
{
public:
	static glm::vec3 moveForward(glm::vec3 pos, float angle, float d) {
		return glm::vec3(pos.x + d*std::sin(angle*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(angle*DEG_TO_RADIAN));
	}

	static glm::vec3 moveRight(glm::vec3 pos, float angle, float d) {
		return glm::vec3(pos.x + d*std::cos(angle*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(angle*DEG_TO_RADIAN));
	}

	//finds the dot product
	static float dot(glm::vec3 A, glm::vec3 B)
	{
		return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
	}

	//returns the length of the vector
	//Doing it this way as there is no length function in the glm class
	static float length(glm::vec3 vec)
	{
		return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	}
};

#endif //UTILS_INCLUDE