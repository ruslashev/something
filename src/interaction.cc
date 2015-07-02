#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "interaction.hh"

Player::Player(Window *wind)
{
	glfwSetCursorPos(wind->win, wind->width/2, wind->height/2);
}

void Player::MoveForward(double dist)
{
	pos += glm::vec3(cos(yaw)*dist,
					 0,
					 sin(yaw)*dist);
}

void Player::Fly(double dist)
{
	pos += glm::vec3(cos(yaw)*cos(pitch)*dist,
					 sin(pitch)*dist,
					 sin(yaw)*cos(pitch)*dist);
}

void Player::Strafe(double dist)
{
	const double rotatedYaw = yaw-M_PI_2;
	pos += glm::vec3(cos(rotatedYaw)*dist, 0, sin(rotatedYaw)*dist);
}

void Player::Rotate(double byPitch, double byYaw)
{
	pitch -= byPitch;
	yaw += byYaw;

	const double precision = 0.0001;
	if (pitch >= M_PI_2-precision)
		pitch = M_PI_2-precision;
	if (pitch <= -M_PI_2+precision)
		pitch = -M_PI_2+precision;
	if (yaw >= 2*M_PI || yaw <= -2*M_PI)
		yaw = 0;
}

glm::mat4 Player::LookAtMat()
{
	return glm::lookAt(
		pos,
		glm::vec3(
			pos.x+cos(yaw)*cos(pitch),
			pos.y+sin(pitch),
			pos.z+sin(yaw)*cos(pitch)
		),
		glm::vec3(0, 1, 0)
	);
}

void Player::Update(Window *wind, double dt)
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
	const int mouseDeltaX = (oldMouseX - wind->width/2);
	const int mouseDeltaY = (oldMouseY - wind->height/2);
	if (oldMouseX && oldMouseY && (mouseDeltaX || mouseDeltaY)) {
		Rotate((double)mouseDeltaY*1.2*dt, (double)mouseDeltaX*1.2*dt);
		glfwSetCursorPos(wind->win, wind->width/2, wind->height/2);
	}
}

