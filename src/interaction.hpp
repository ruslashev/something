#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct AABB { glm::vec3 pos; unsigned int w, h, d; };

class BaseEntity
{
public:
	int health; // value < 0 means undead
	glm::vec3 pos;
	double pitch, yaw;

	void Redraw();
	void Update();
};

class Player : public BaseEntity
{
public:
	Player(Window *wind);
	void MoveForward(double dist);
	void Strafe(double dist);
	void Rotate(double byPitch, double byYaw);
	glm::mat4 LookAtMat();
	void Update(Window *wind, double dt);
};

#endif

