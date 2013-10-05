#include "camera.hpp"

void Camera::MoveForward(double dist)
{
	position += glm::vec3(cos(yaw)*cos(pitch)*dist,
			sin(pitch)*dist,
			sin(yaw)*cos(pitch)*dist);
}

void Camera::Rotate(double byPitch, double byYaw)
{
	pitch -= byPitch;
	yaw += byYaw;

	if (pitch >= M_PI_2-0.001)
		pitch = M_PI_2-0.001;
	if (pitch <= -M_PI_2+0.001)
		pitch = -M_PI_2+0.001;
	if (yaw >= 2*M_PI || yaw <= -2*M_PI)
		yaw = 0;
}

glm::mat4 Camera::LookAtMat()
{
	return glm::lookAt(
		position,
		glm::vec3(
			position.x+cos(yaw)*cos(pitch),
			position.y+sin(pitch),
			position.z+sin(yaw)*cos(pitch)
		),
		glm::vec3(0, 1, 0)
	);
}

void Camera::Update(GLFWwindow **win, double dt)
{
	if (glfwGetKey(*win, 'W'))
		MoveForward(10*dt);
	if (glfwGetKey(*win, 'S'))
		MoveForward(-10*dt);

	double mouseX, mouseY;
	glfwGetCursorPos(*win, &mouseX, &mouseY);
	glfwSetCursorPos(*win, 800/2, 600/2);
	double mouseDeltaX = (mouseX - 800/2);
	double mouseDeltaY = (mouseY - 600/2);
	if (mouseDeltaX || mouseDeltaY)
		Rotate(mouseDeltaY*dt, mouseDeltaX*dt);
}

