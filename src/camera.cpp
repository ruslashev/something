#include "window.hpp"
#include "camera.hpp"

void Camera::MoveForward(double dist)
{
	position += glm::vec3(cos(yaw)*cos(pitch)*dist,
			sin(pitch)*dist,
			sin(yaw)*cos(pitch)*dist);
}

void Camera::Strafe(double dist)
{
	const double rotatedYaw = yaw-M_PI_2;
	position += glm::vec3(cos(rotatedYaw)*dist, 0, sin(rotatedYaw)*dist);
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

void Camera::Update(Window *wind, double dt)
{
	if (glfwGetKey(wind->win, 'W'))
		MoveForward(10*dt);
	if (glfwGetKey(wind->win, 'S'))
		MoveForward(-10*dt);

	if (glfwGetKey(wind->win, 'A'))
		Strafe(10*dt);
	if (glfwGetKey(wind->win, 'D'))
		Strafe(-10*dt);

	double oldMouseX, oldMouseY;
	glfwGetCursorPos(wind->win, &oldMouseX, &oldMouseY);
	int mouseDeltaX = (oldMouseX - wind->width/2);
	int mouseDeltaY = (oldMouseY - wind->height/2);
	if (oldMouseX && oldMouseY && (mouseDeltaX || mouseDeltaY))
		Rotate((double)mouseDeltaY*1.2*dt, (double)mouseDeltaX*1.2*dt);
	glfwSetCursorPos(wind->win, wind->width/2, wind->height/2);
}

