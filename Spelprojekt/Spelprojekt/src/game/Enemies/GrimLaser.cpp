#include "GrimLaser.h"
#include "../map.h"

GrimLaser::GrimLaser(glm::vec2 firstPos, bool vert)
{
	initPos = firstPos; //This is never actually used, only copy constructor matters
	moveTo(initPos.x, initPos.y);
	deathTimer = 1.5f;
	audibleDistance = 10.0f;

	contentIndex = WorldID::pillar;

	collideRect = new Rect();
	vertical = vert;
	if (vertical)
		collideRect->initGameObjectRect(&worldMat, 2, 70);
	else
		collideRect->initGameObjectRect(&worldMat, 70, 2);
}

void GrimLaser::init()
{
	//Not needed since it's a visitor-only projectile
}

int GrimLaser::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	deathTimer -= 1.0*deltaTime;
	if (deathTimer < FLT_EPSILON)
		alive = false;
		return 1;
	if (alive)
		return 0;
}

void GrimLaser::hit(int damage, bool playerRightOfEnemy)
{
}