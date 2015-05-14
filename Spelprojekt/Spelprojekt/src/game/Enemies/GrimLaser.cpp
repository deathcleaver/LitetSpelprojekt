#include "GrimLaser.h"
#include "../map.h"

GrimLaser::GrimLaser(glm::vec2 firstPos, bool vert)
{
	initPos = firstPos; 
	moveTo(initPos.x, initPos.y);
	deathTimer = 1.5f;
	audibleDistance = 10.0f;
	alive = true;

	contentIndex = WorldID::pillar;

	collideRect = new Rect();
	vertical = vert;
	if (vertical)
		collideRect->initGameObjectRect(&worldMat, 2, 70);
	else
	{
		collideRect->initGameObjectRect(&worldMat, 70, 2);
		rotateTo(0, 0, 3.141592654f / 2.0f);
	}
}

void GrimLaser::init()
{
	//Not needed since it's a visitor-only projectile
}

int GrimLaser::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	collideRect->update();
	deathTimer -= 1.0*deltaTime;
	if (deathTimer < FLT_EPSILON)
	{
		alive = false;
		return 1;
	}
	if (alive)
		return 0;
}

void GrimLaser::hit(int damage, bool playerRightOfEnemy)
{
}