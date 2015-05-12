#include "Grim.h"
#include "../mapChunk.h"
#include "../map.h"

Grim::Grim(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::grim_white;
	health = 4;
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2.5f);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 2.5f, 2.8f);
}

Grim::~Grim()
{
	delete hurtRect;
}

void Grim::init()
{
	if (!isInit)
	{
		contentIndex = EnemyID::grim_white;
		worldMat = glm::mat4(1);
		isInit = true;
		moveTo(initPos.x, initPos.y);
		invulnTimer = 0.0f;
		facingRight = true;
		alive = true;
		health = 4;
		collideRect->update();
		Audio::getAudio().playSoundAtPos(SoundID::boss_bat_attack, readPos(), audibleDistance + 2, false);//boss_bat_attack
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

int Grim::update(float deltaTime, Map* map, glm::vec3 playerPos)
{	
	hurtRect->update();
	return 0;
}

bool Grim::isInitiated()
{
	return isInit;
}

void Grim::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health > 0)
		{
			invulnTimer = 1.0f;
			Debug::DebugOutput("Boss took damage \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_bat_hurt, readPos(), audibleDistance, false);//boss_bat_hurt
		}
		else if (alive == true)
		{
			alive = false;
			Debug::DebugOutput("Boss is dead \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_bat_death, readPos(), audibleDistance, false);//boss_bat_death
		}
	}
}

bool Grim::isBlinking()
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

glm::vec2 Grim::plsGiveBossPos()
{
	return initPos;
}

glm::vec2 Grim::getHandPos()
{
	return glm::vec2(-2, 7);
}