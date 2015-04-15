#ifndef PLAYER_H
#define PLAYER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "../UserInput.h"
#include "gameObject.h"

class Player : public GameObject
{
private:
	float timepass = 0.0f;
	// camera variables
	int dir;

public:
	Player(){};
	~Player(){};
	void init();
	int update(float deltaTime);
	int update(UserInput* userInput, float deltaTime);
	int getDir();
};

#endif