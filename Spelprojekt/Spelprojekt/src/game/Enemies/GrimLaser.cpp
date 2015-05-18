#include "GrimLaser.h"
#include "../map.h"

GrimLaser::GrimLaser(glm::vec2 firstPos, bool vert)
{
	initPos = firstPos; 
	moveTo(initPos.x, initPos.y);
	deathTimer = 1.5f;
	audibleDistance = 10.0f;
	alive = true;
	Audio::getAudio().playSoundAtPos(SoundID::boss_grim_laser, glm::vec3(initPos.x, initPos.y,0), audibleDistance, false);

	collideRect = new Rect();
	vertical = vert;
	if (vertical)
	{
		collideRect->initGameObjectRect(&worldMat, 0.0, 70);
		contentIndex = EnemyID::grim_lazer_v;
		scaleFactor(0.05f, 1.0f, 0.05f);
	}
	else
	{
		collideRect->initGameObjectRect(&worldMat, 70, 0.0);
		contentIndex = EnemyID::grim_lazer_h;
		scaleFactor(1.0f, 0.05f, 0.05f);
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
		width += 1.5f*deltaTime;
		if (vertical)
		{
			collideRect->initGameObjectRect(&worldMat, width, 70);
			scaleAD(1.5f*deltaTime, 0.0f, 1.5f*deltaTime);
		}
		else
		{
			collideRect->initGameObjectRect(&worldMat, 70, width);
			scaleAD(0.0f, 1.5f*deltaTime, 1.5f*deltaTime);
		}
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