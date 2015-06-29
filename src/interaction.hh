#ifndef INTERACTION_HH
#define INTERACTION_HH

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	void Fly(double dist);
	void Strafe(double dist);
	void Rotate(double byPitch, double byYaw);
	glm::mat4 LookAtMat();
	void Update(Window *wind, double dt);
};

#endif

