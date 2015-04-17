#include "enemy.h"

Rect* Enemy::getRekt()
{
	collideRect->update();
	return collideRect;
}

bool Enemy::isAlive()
{
	return alive;
}

glm::vec3 Enemy::getPos()
{
	return readPos();
}

Enemy::~Enemy()
{
	delete collideRect;
}