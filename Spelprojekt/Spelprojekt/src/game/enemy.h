#ifndef ENEMY_H
#define ENEMY_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include <string>

#include "gameObject.h"
#include "rect.h"

#include "../legend.h"

class Map;
class MapChunk;

class Enemy : public GameObject
{
protected:
	bool visitor = false;
	bool alive;
	glm::vec2 initPos;
	int health;
	float audibleDistance = 2.5f;
	Rect* collideRect;

	bool collidesWithWorld(Map* map);
public:
	Rect* getRekt();
	virtual void init() = 0;
	virtual ~Enemy();
	virtual int update(float deltaTime, Map* map, glm::vec3 playerPos) = 0;
	virtual void hit(int damage, bool playerRightOfEnemy) = 0;
	virtual bool isBlinking();
	virtual std::string getType() = 0;
	bool isAlive();

	virtual bool isInitiated();
	void setVisitor();
	bool isVisitor();
	void diePls();

	virtual glm::vec2 plsGiveBossPos();
};

#endif