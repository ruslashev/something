#include "window.hpp"
#include "camera.hpp"

Camera::Camera(Window *win)
{
	glfwSetCursorPos(win->win, win->width/2, win->height/2);
}

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

void Camera::Update(Window *win, double dt)
{
	if (glfwGetKey(win->win, 'W'))
		MoveForward(10*dt);
	if (glfwGetKey(win->win, 'S'))
		MoveForward(-10*dt);

	if (glfwGetKey(win->win, 'A'))
		Strafe(10*dt);
	if (glfwGetKey(win->win, 'D'))
		Strafe(-10*dt);

	if (glfwGetKey(win->win, 'P'))
		printf("%10f %10f\n", pitch, yaw);

	double mouseX, mouseY;
	glfwGetCursorPos(win->win, &mouseX, &mouseY);
	glfwSetCursorPos(win->win, win->width/2, win->height/2);
	double mouseDeltaX = (mouseX - win->width/2);
	double mouseDeltaY = (mouseY - win->height/2);
	if (mouseDeltaX || mouseDeltaY)
		Rotate(mouseDeltaY*1.2*dt, mouseDeltaX*1.2*dt);
}

