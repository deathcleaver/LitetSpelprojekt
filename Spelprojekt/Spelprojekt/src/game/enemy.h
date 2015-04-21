#ifndef ENEMY_H
#define ENEMY_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include <string>

#include "gameObject.h"
#include "rect.h"

class MapChunk;

class Enemy : public GameObject
{
protected:
	bool alive;
	glm::vec2 initPos;
	int health;
	Rect* collideRect;
public:
	Rect* getRekt();
	virtual void init() = 0;
	virtual ~Enemy();
	virtual int update(float deltaTime, MapChunk* chunk) = 0;
	virtual void hit(int damage, bool playerRightOfEnemy) = 0;
	virtual bool isBlinking();
	bool isAlive();
	glm::vec3 getPos();

	virtual std::string isBoss();
};

#endif