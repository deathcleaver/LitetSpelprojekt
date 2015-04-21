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

bool Enemy::isBlinking()
{
	return false;
}

std::string Enemy::isBoss()
{
	return "Notboss";
}