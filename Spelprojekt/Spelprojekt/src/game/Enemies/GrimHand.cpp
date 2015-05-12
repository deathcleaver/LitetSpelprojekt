#include "GrimHand.h"
#include "../map.h"

GrimHand::GrimHand(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = 1;
	audibleDistance = 2.5f;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

GrimHand::GrimHand(GrimHand* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	initPos = copy->initPos;
	glm::vec3 pos = copy->readPos();
	moveTo(pos.x, pos.y);
	alive = true;
	facingRight = copy->facingRight;
	health = 1;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

void GrimHand::init()
{
	invulnTimer = 1.0f;
	stateTimer = 4.0f;
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 1;
	state = -1;

	collideRect->update();
}

int GrimHand::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (state == -1)
	{
		stateTimer -= 1.0f*deltaTime;
		if (stateTimer < 2.0f)
		{
			if (invulnTimer > FLT_EPSILON)
				invulnTimer -= 1.0f*deltaTime;
		}
		if (stateTimer < FLT_EPSILON)
		{
			state = 0;
		}
	}
	return 0;
}

void GrimHand::hit(int damage, bool playerRightOfEnemy)
{
	health -= damage;
	if (health <= 0)
	{
		Audio::getAudio().playSoundAtPos(SoundID::boss_bat_death, readPos(), audibleDistance, false); //boss_GrimHand_death
		alive = false;
	}
}

bool GrimHand::isBlinking()
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