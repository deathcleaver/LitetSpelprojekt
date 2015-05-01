#include "enemy.h"
#include "map.h"

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

bool Enemy::isBlinking()
{
	return false;
}

bool Enemy::isInitiated()
{
	return true;
}

void Enemy::setVisitor()
{
	visitor = true;
}

bool Enemy::isVisitor()
{
	return visitor;
}

void Enemy::diePls()
{
	alive = false;
}

glm::vec2 Enemy::plsGiveBossPos()
{
	return glm::vec2(0, 0);
}

bool Enemy::collidesWithWorld(Map* map)
{
	collideRect->update();
	return map->collideMap(collideRect, readPos());
}