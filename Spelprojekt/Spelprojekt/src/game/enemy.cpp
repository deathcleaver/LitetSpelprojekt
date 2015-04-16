#include "enemy.h"

Rect* Enemy::getRekt()
{
	return collideRect;
}

bool Enemy::isAlive()
{
	return alive;
}

Enemy::~Enemy()
{
	delete collideRect;
}