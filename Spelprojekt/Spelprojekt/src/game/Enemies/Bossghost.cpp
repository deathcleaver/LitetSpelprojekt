#include "Bossghost.h"
#include "../mapChunk.h"
#include "../map.h"

Bossghost::Bossghost(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::batboss;
	health = 6;
	speed = 4.0f;
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;
	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2.5);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 2.5, 2.8);
}

Bossghost::~Bossghost()
{
	delete hurtRect;
}

void Bossghost::init()
{
	if (!isInit)
	{
		inMirror = true;
		worldMat = glm::mat4(1);
		isInit = true;
		moveTo(initPos.x, initPos.y);
		invulnTimer = 0.0f;
		movementScale = 0.0f;
		facingRight = true;
		alive = true;
		health = 6;
		collideRect->update();
		Audio::getAudio().playSoundAtPos(SoundID::boss_bat_attack, readPos(), audibleDistance + 2, false);//boss_bat_attack

		state = -1;
		stateTimer = 5.0f;
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

int Bossghost::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec3 pos;
	if (state == -1) //Spawn state
	{
		stateTimer -= 1.0*deltaTime;
		if (stateTimer < 2.0f)
		{
			scaleAD(1.0f*deltaTime, 1.0f*deltaTime, 1.0f*deltaTime);
		}
		else
			moveTo(playerPos.x, playerPos.y);
		if (stateTimer < FLT_EPSILON)
		{
			state = 0;
			pos = readPos();
			dirToMid = initPos - glm::vec2(pos);
			dirToMid = normalize(dirToMid);
		}
	}
	else if (state == 0) //Jumping out of mirror state
	{
		pos = readPos();
		if (pos.x - initPos.x < FLT_EPSILON && initPos.x - pos.x > -FLT_EPSILON &&
			pos.y - initPos.y < FLT_EPSILON && initPos.y - pos.y > -FLT_EPSILON)
		{
			if (pos.z > -2.0f && inMirror)
				translate(0, 0, -2.0f*deltaTime);
			else
			{
				translate(0, 0, 2.0f*deltaTime);
				inMirror = false;
			}
			pos = readPos();
			if (pos.z > -FLT_EPSILON)
			{
				state = 1;
			}
		}
		else
			translate(dirToMid.x*speed*2.0f*deltaTime, dirToMid.y*speed*2.0f*deltaTime);
	}
	else if (state == 1) //Flying outside of mirror state
	{

	}
	else if (state == 2) //Jumping into mirror state
	{
	}
	else if (state == 3) //Flying inside mirror state
	{
	}
	hurtRect->update();
	return 0;
}

bool Bossghost::isInitiated()
{
	return isInit;
}

void Bossghost::hit(int damage, bool playerRightOfEnemy)
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

bool Bossghost::isBlinking()
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

glm::vec2 Bossghost::plsGiveBossPos()
{
	return initPos;
}