#include "Grim.h"
#include "../mapChunk.h"
#include "../map.h"
#include "ArcaneMissile.h"

Grim::Grim(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::grim_white;
	health = 2;
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0, 0);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 2.5f, 2.5f);
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
		health = 2;
		collideRect->update();
		Audio::getAudio().playSound(SoundID::boss_grim_intro, false);
		Audio::getAudio().playMusic(MusicID::lastboss_stage1);

		mode = 0;
		state = -1;
		stateTimer = 6.0f;
		
		speed = 6.0f;
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

int Grim::update(float deltaTime, Map* map, glm::vec3 playerPos)
{	
	if (mode < 4)
	{
		if (state == -1)
		{
			stateTimer -= 1.0f*deltaTime;
			if (stateTimer < FLT_EPSILON)
			{
				stateTimer = 0.5f;
				state = 0;
			}
		}
		else if (state == 0)
		{
			stateTimer -= 1.0f*deltaTime;
			if (stateTimer < FLT_EPSILON)
			{
				fireBall(map, playerPos, false);
				fireBall(map, playerPos, true);
				stateTimer = 2.0f;
			}
		}
	}
	else if (mode == 4)
	{
		Audio::getAudio().playMusicFade(MusicID::lastboss_stage2, deltaTime);
		if (invulnTimer < FLT_EPSILON)
		{
			mode = 5; //BEHOLD TRUE POWER
			Audio::getAudio().playSound(SoundID::boss_grim_transform, false);
			contentIndex = contentIndex + 1;
			stateTimer = 0.5f;
			state = -1;
		}
	}
	else if (mode == 5)
	{
		if (state == -1)
		{
			stateTimer = -1.0f;
			if (stateTimer < FLT_EPSILON)
			{
				state = 0;
				calcDir(0);
				stateTimer = 7.0f;
				fireBallTimer = 0.0f;
			}
		}
		if (state == 0)
		{
			if (reachedDestination())
			{
				calcDir((headingTo + 1) % 4);
			}
			else
				translate(dirToFly.x*speed*deltaTime, dirToFly.y*speed*deltaTime);

			stateTimer -= 1.0f*deltaTime;
			if (stateTimer < FLT_EPSILON)
			{
				//state = rand() % 2 + 1; //Vertical or horizontal slash
			}

			fireBallTimer -= 1.0f*deltaTime;
			if (fireBallTimer < FLT_EPSILON)
			{
				fireBall(map, playerPos, true);
				fireBallTimer = 1.5f;
			}
		}
	}
	hurtRect->update();
	if (invulnTimer > FLT_EPSILON)
		invulnTimer -= 1.0*deltaTime;
	return mode;
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
			Audio::getAudio().playSound(SoundID::boss_grim_hurt, false);
		}
		else if (mode == 0) //Switch to Force
		{
			health = 2;
			invulnTimer = 1.0f;
			mode = 1;
			contentIndex = contentIndex + 1;
		}
		else if (mode == 1) //Switch to Range
		{
			health = 2;
			invulnTimer = 1.0f;
			mode = 2;
			contentIndex = contentIndex + 1;
		}
		else if (mode == 2) //Switch to Spark
		{
			health = 2;
			invulnTimer = 1.0f;
			mode = 3;
			contentIndex = contentIndex + 1;
		}
		else if (mode == 3) //Switch to Dying
		{
			health = 6;
			Audio::getAudio().playSound(SoundID::boss_grim_death1, false);
			invulnTimer = 4.0f;
			mode = 4;
			contentIndex = contentIndex + 1;
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

void Grim::fireBall(Map* map, glm::vec3 playerPos, bool rightEye)
{
	glm::vec2 pos = glm::vec2(readPos());
	if (mode != 5)
	{
		if (!rightEye)
			pos.x = initPos.x - 1.5f;
		else
			pos.x = initPos.x + 1.5f;
		pos.y = initPos.y + 4.0f;
	}
	ArcaneMissile* pewpew = new ArcaneMissile(pos);
	pewpew->setVisitor();
	pewpew->setEffect(glm::vec3(0.8f, 0.8f, 0.2f), false, true, false, 40);
	glm::vec2 dir;
	dir = glm::vec2(playerPos) - pos;
	dir.x = dir.x + (rand() % 3 - 1);
	dir.y = dir.y + (rand() % 3 - 1);
	dir = normalize(dir);
	pewpew->setDirection(dir);
	map->findNewHome(pewpew);
	delete pewpew;
}

Rect* Grim::getRekt()
{
	if (state == 5)
		return collideRect;
	return 0;
}

void Grim::calcDir(int position)
{
	glm::vec2 pos = glm::vec2(readPos());
	switch (position)
	{
	case -1:
		currentGoal = initPos;
		break;
	case 0: //Top
		currentGoal.x = initPos.x;
		currentGoal.y = initPos.y + 4.0f;
		break;
	case 1: //Right
		currentGoal.x = initPos.x + 7.0f;
		currentGoal.y = initPos.y;
		break;
	case 2: //Bot
		currentGoal.x = initPos.x;
		currentGoal.y = initPos.y - 4.0f;
		break;
	case 3: //Left
		currentGoal.x = initPos.x -7.0f;
		currentGoal.y = initPos.y;
		break;
	}
	headingTo = position;
	dirToFly = currentGoal - pos;
	dirToFly = normalize(dirToFly);
}

bool Grim::reachedDestination()
{
	glm::vec3 pos = readPos();
	if (pos.x < currentGoal.x + 1.0f && pos.x > currentGoal.x - 1.0f &&
		pos.y < currentGoal.y + 1.0f && pos.y > currentGoal.y - 1.0f)
		return true;
	return false;
}