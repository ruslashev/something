#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::vec3 position;
	double pitch, yaw;

	void Update(Window *wind, double dt);
	void MoveForward(double dist);
	void Strafe(double dist);
	void Rotate(double byPitch, double byYaw);

	glm::mat4 LookAtMat();
};

#endif

