#include "Webshot.h"
#include "../Web.h"
#include "../map.h"

Webshot::Webshot(glm::vec2 firstPos)
{
	initPos = firstPos; //This is never actually used, only copy constructor matters
}

Webshot::Webshot(Webshot* copy)
{
	worldMat = copy->worldMat;
	initPos = copy->initPos;
	moveTo(initPos.x, initPos.y);
	contentIndex = 6; //same as Web
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);

	health = 1;
	speed = 5.0f;

	direction = copy->direction;
}

void Webshot::setDirection(glm::vec2 dir)
{
	direction = dir;
}

void Webshot::init()
{
	//Not needed since it's a visitor-only projectile
}

int Webshot::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (!alive)
		return 1;
	translate(direction.x*speed*deltaTime, direction.y*speed*deltaTime);
	collideRect->update();
	if (collidesWithWorld(map))
	{
		Web* web = new Web(glm::vec2(readPos()));
		web->setVisitor();
		map->findNewHome(web);
		return 1;
	}
	return 0;
}

void Webshot::hit(int damage, bool playerRightOfEnemy)
{
	health -= damage;
	if (health <= 0)
	{
		alive = false;
	}
}