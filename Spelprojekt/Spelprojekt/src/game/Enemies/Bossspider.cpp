#include "Bossspider.h"
#include "../mapChunk.h"
#include "../map.h"

Bossspider::Bossspider(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = 1;
	health = 4;
	speed = 4.0f;
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2.5);
}

void Bossspider::init()
{
	if (!isInit)
	{
		worldMat = glm::mat4(1);
		scaleFactor(4, 4, 4);
		isInit = true;
		moveTo(initPos.x-0.5f, initPos.y+0.5f, -10.0f);
		invulnTimer = 0.0f;
		facingRight = true;
		alive = true;
		health = 4;
		collideRect->update();
		Audio::getAudio().playSoundAtPos(9, readPos(), audibleDistance + 2, false);//boss_bat_attack
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

int Bossspider::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	
	return 0;
}

bool Bossspider::isInitiated()
{
	return isInit;
}

void Bossspider::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health > 0)
		{
			invulnTimer = 1.0f;
			printf("Boss took damage \n");
			Audio::getAudio().playSoundAtPos(10, readPos(), audibleDistance, false);//boss_bat_hurt
		}
		else if (alive == true)
		{
			alive = false;
			printf("Boss is dead \n");
			Audio::getAudio().playSoundAtPos(11, readPos(), audibleDistance, false);//boss_bat_death
		}
	}
}

bool Bossspider::isBlinking()
{
	if (invulnTimer > 0.0f)
	{
		int check = int(invulnTimer * 10);
		if (check % 3)
		{
			return true;
		}
	}
	return false;
}

glm::vec2 Bossspider::plsGiveBossPos()
{
	return initPos;
}