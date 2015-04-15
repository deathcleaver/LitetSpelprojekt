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
	// camera variables
	int dir;


	Rect* collideRect = 0;
	Rect* testRect = 0;
	glm::mat4 testMat;

public:
	Player(){};
	~Player(){};
	void init();
	int update(float deltaTime);
	int update(UserInput* userInput, Map* map, float deltaTime);
	int getDir();

};

#endif