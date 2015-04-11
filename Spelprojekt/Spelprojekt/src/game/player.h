#ifndef PLAYER_H
#define PLAYER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "gameObject.h"

class Player : public GameObject
{
private:
	float timepass = 0.0f;
public:
	Player(){};
	~Player(){};
	void init();
	void update(float deltaTime);
};

#endif