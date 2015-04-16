#ifndef PLAYER_H
#define PLAYER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "gameObject.h"
#include "rect.h"
#include "map.h"
#include "mapChunk.h"
#include "../UserInput.h"

class Player : public GameObject
{
private:
	float timepass = 0.0f;

	//movement
	vec2 speed;
	vec2 maxSpeed;
	vec2 acceleration;
	bool jumping;
	
	Rect* collideRect = 0;

public:
	Player(){};
	~Player();
	void init();

	int update(UserInput* userInput, Map* map, float deltaTime);
	vec2 getSpeed();
};

#endif