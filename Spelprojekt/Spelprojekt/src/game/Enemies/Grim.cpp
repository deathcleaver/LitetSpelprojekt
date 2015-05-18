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
	audibleDistance = 20.0f;

	invulnTimer = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2);
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
	if (mode == 10)
	{
		mode = 5;
		state = 0;
		calcDir(0);
		fireBallTimer = 0.0f;
		return 10;
	}
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
		if (invulnTimer < FLT_EPSILON)
		{
			Audio::getAudio().playMusic(MusicID::lastboss_stage2_loop);
			mode = 5; //BEHOLD TRUE POWER
			Audio::getAudio().playSound(SoundID::boss_grim_transform, false);
			contentIndex = contentIndex + 1;
			stateTimer = 0.5f;
			state = -1;
		}
	}
	else if (mode > 4 && mode != 8)
	{
		collideRect->update();
		if (state == -1)
		{
			stateTimer = -1.0f;
			if (stateTimer < FLT_EPSILON)
			{
				state = 0;
				calcDir(0);
				stateTimer = 5.0f;
				fireBallTimer = 0.0f;
			}
		}
		else if (state == 0) //Flying around shooting shit
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
				verticalAttack = rand() % 2; //Horizontal or vertical slash
				doubleSwipe = false; //When at low HP, slashes twice
				state = 1;
				stateTimer = 2.5f;
				prepspeed.x = prepspeed.y = 0;
			}

			fireBallTimer -= 1.0f*deltaTime;
			if (fireBallTimer < FLT_EPSILON)
			{
				fireBall(map, playerPos, true);
				fireBallTimer = 1.5f;
			}
		}
		else if (state == 1) //Attack preparation
		{
			glm::vec3 pos = readPos();
			if (!verticalAttack)
			{
				if (dirToFly.y < 0.0f && playerPos.y > pos.y)
					prepspeed.y = -prepspeed.y;
				if (dirToFly.y > 0.0f && playerPos.y < pos.y)
					prepspeed.y = -prepspeed.y;
				prepspeed.y += 0.5f;
				if (prepspeed.y > 9.0f)
					prepspeed.y = 9.0f;

				if (dirToFly.x < 0.0f && playerPos.x-6.0f > pos.x)
					prepspeed.x = -prepspeed.y;
				if (dirToFly.x > 0.0f && playerPos.x-6.0f < pos.x)
					prepspeed.x = -prepspeed.y;
				prepspeed.x += 0.2f;
				if (prepspeed.x > 7.0f)
					prepspeed.x = 7.0f;
			}
			else
			{
				if (dirToFly.x < 0.0f && playerPos.x > pos.x)
					prepspeed.x = -prepspeed.y;
				if (dirToFly.x > 0.0f && playerPos.x < pos.x)
					prepspeed.x = -prepspeed.y;
				prepspeed.x += 0.5f;
				if (prepspeed.x > 9.0f)
					prepspeed.x = 9.0f;

				if (dirToFly.y < 0.0f && playerPos.y+6.0f > pos.y)
					prepspeed.y = -prepspeed.y;
				if (dirToFly.y > 0.0f && playerPos.y+6.0f < pos.y)
					prepspeed.y = -prepspeed.y;
				prepspeed.y += 0.2f;
				if (prepspeed.y > 7.0f)
					prepspeed.y = 7.0f;
			}

			if (!verticalAttack)
				calcDir(glm::vec2(playerPos.x - 6.0f, playerPos.y));
			else
				calcDir(glm::vec2(playerPos.x, playerPos.y + 6.0f));

			translate(prepspeed.x*dirToFly.x*deltaTime, prepspeed.y*dirToFly.y*deltaTime);

			stateTimer -= 1.0f*deltaTime;
			if (stateTimer < FLT_EPSILON)
			{
				state = 2;
				stateTimer = 2.0f;
			}
		}
		else if (state == 2) //Slash'em
		{
			stateTimer -= 1.0f*deltaTime;
			if (stateTimer < 1.5f)
			{
				if (!verticalAttack)
					mode = 6;
				else
					mode = 7;
			}
			if (stateTimer < FLT_EPSILON)
			{
				if (!doubleSwipe && health < 3)
				{
					doubleSwipe = true;
					verticalAttack = !verticalAttack;
					state = 1;
					stateTimer = 1.0f;
					prepspeed.x = prepspeed.y = 0;
					mode = 5;
				}
				else
				{
					mode = 5;
					state = 0;
					calcDir((headingTo + 1) % 4);
					stateTimer = 5.0f;
					fireBallTimer = 1.0f;
				}
			}
		}
	}
	else if (mode == 8)
	{
		state = 0;
		if (invulnTimer <= 0)
		{
			Audio::getAudio().playSound(SoundID::boss_grim_death2, false);
			alive = false;
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
		if (mode < 5)
			Audio::getAudio().playSound(SoundID::boss_grim_hurt, false);
		else if (health > 0)
			Audio::getAudio().playSound(SoundID::boss_grim_hurt, false);

		if (health > 0)
		{
			invulnTimer = 1.0f;
			Debug::DebugOutput("Boss took damage \n");
			if (mode == 5 && state != 1 && state != 2)
			{
				mode = 10;
			}
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
			invulnTimer = 5.0f;
			mode = 4;
			Audio::getAudio().playMusic(MusicID::lastboss_stage2_intro);
			contentIndex = contentIndex + 1;
		}
		else if (mode == 5)
		{
			if (soDefeated == false)
			{
				Audio::getAudio().playMusic(-1);
				Audio::getAudio().playSound(SoundID::boss_clear, false);//boss_defeted
				Audio::getAudio().playSound(SoundID::boss_grim_so_defeated, false);
				invulnTimer = 6.0f;
				soDefeated = true;
			}
				
			mode = 8;
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
	if (mode > 4)
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

void Grim::calcDir(glm::vec2 goal)
{
	currentGoal = goal;
	glm::vec2 pos = glm::vec2(readPos());
	dirToFly = currentGoal - pos;
	dirToFly = normalize(dirToFly);
}