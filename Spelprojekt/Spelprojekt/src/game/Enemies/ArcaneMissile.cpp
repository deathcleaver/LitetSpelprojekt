#include "ArcaneMissile.h"
#include "../Web.h"
#include "../map.h"

ArcaneMissile::ArcaneMissile(glm::vec2 firstPos)
{
	initPos = firstPos; //This is never actually used, only copy constructor matters
	moveTo(initPos.x, initPos.y);
}

ArcaneMissile::ArcaneMissile(ArcaneMissile* copy)
{
	alive = true;
	scaleFactor(0.01, 0.01, 0.01);
	worldMat = copy->worldMat;
	initPos = copy->initPos;
	moveTo(initPos.x, initPos.y);
	contentIndex = EnemyID::bat; //same as Web
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.3, 0.3);

	health = 1;
	speed = 0.1f;

	direction = copy->direction;
}

void ArcaneMissile::setDirection(glm::vec2 dir)
{
	direction = dir;
}

void ArcaneMissile::init()
{
	//Not needed since it's a visitor-only projectile
}

int ArcaneMissile::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (!alive)
		return 1;
	speed += speed*0.05;
	translate(direction.x*speed*deltaTime, direction.y*speed*deltaTime);
	if (collidesWithWorld(map))
	{
		return 1;
	}
	collideRect->update();
	return 0;
}

void ArcaneMissile::hit(int damage, bool playerRightOfEnemy)
{
	health -= damage;
	if (health <= 0)
	{
		alive = false;
	}
}