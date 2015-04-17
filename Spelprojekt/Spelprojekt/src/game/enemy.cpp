#include "enemy.h"

Rect* Enemy::getRekt()
{
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

void Enemy::reset()
{
	moveTo(initPos.x, initPos.y);
	alive = true;
	health = 1;
}