#include "GrimLaser.h"
#include "../map.h"

GrimLaser::GrimLaser(glm::vec2 firstPos, bool vert)
{
	initPos = firstPos; 
	moveTo(initPos.x, initPos.y);
	deathTimer = 1.5f;
	audibleDistance = 10.0f;
	alive = true;

	contentIndex = EnemyID::grim_lazer;

	collideRect = new Rect();
	vertical = vert;
	if (vertical)
		collideRect->initGameObjectRect(&worldMat, 0.0, 70);
	else
	{
		collideRect->initGameObjectRect(&worldMat, 70, 0.0);
		rotateTo(0, 0, 3.141592654f / 2.0f);
	}
	width = 0.0f;
}

void GrimLaser::init()
{
	//Not needed since it's a visitor-only projectile
}

int GrimLaser::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	deathTimer -= 1.0*deltaTime;
	if (deathTimer < 1.0f)
	{
		width += 2.0f*deltaTime;
		if (vertical)
			collideRect->initGameObjectRect(&worldMat, width, 70);
		else
			collideRect->initGameObjectRect(&worldMat, 70, width);
	}
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

bool GrimLaser::isBlinking()
{
	if (deathTimer < 1.0f)
		return true;
	return false;
}