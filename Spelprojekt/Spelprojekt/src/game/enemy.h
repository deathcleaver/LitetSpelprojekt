#ifndef ENEMY_H
#define ENEMY_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "gameObject.h"

class Enemy : public GameObject
{
protected:
	bool alive;
	glm::vec2 initPos;
	int health;
public:
	virtual void init() = 0;
	virtual int update(float deltaTime) = 0;
	virtual void hit(int damage) = 0;
};

#endif