#include "Bossbat.h"
#include "../mapChunk.h"
#include "../map.h"
#include "Bat.h"
#include "EchoLocation.h"

Bossbat::Bossbat(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::batboss;
	health = 4;
	speed = 4.0f + GameConfig::get().configDifficulty * 2; // + (0->2)
	slow = false;
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;
	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2.5f);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 2.5f, 2.8f);

	spawnsLeft = 0;
	batTimer = 0.0f;
	charging = false;
	chargeTimer = 4.0f - GameConfig::get().configDifficulty;

	isShootingEchos = false;
}

Bossbat::~Bossbat()
{
	delete hurtRect;
}

void Bossbat::init()
{
	if (!isInit)
	{
		worldMat = glm::mat4(1);
		isInit = true;
		moveTo(initPos.x-0.5f, initPos.y+0.5f, -10.0f);
		invulnTimer = 0.0f;
		movementScale = 0.0f;
		facingRight = true;
		alive = true;
		health = 4;
		slow = false;
		collideRect->update();
		spawnsLeft = 3;
		batTimer = 0.0f;
		chargeTimer = 4.0f - GameConfig::get().configDifficulty;
		echoTimer = 8.0f;
		charging = false;
		returnPos = chargePos = readPos();
		hasTurned = false;
		rotateTo(0, 3.1415927f / 2.0f, 0);
		Audio::getAudio().playMusic(MusicID::boss_battle_bat);
		Audio::getAudio().playSoundAtPos(SoundID::boss_bat_attack, readPos(), audibleDistance + 2, false);//boss_bat_attack
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

void Bossbat::spawnBat(Map* map, float deltaTime)
{
	if (spawnsLeft > 0)
	{
		batTimer -= 1.0f*deltaTime;
		if (batTimer < FLT_EPSILON)
		{
			glm::vec3 pos = readPos();
			Bat* newBat = new Bat(glm::vec2(pos.x, pos.y-2.5));
			newBat->setFacing(facingRight);
			newBat->setVisitor();
			map->findNewHome(newBat);
			delete newBat;
			spawnsLeft--;
			if (spawnsLeft > 0)
				batTimer = 0.3f;
		}
	}
}

void Bossbat::echoLocation(Map* map, glm::vec3 playerPos)
{
	glm::vec3 pos = readPos();
	glm::vec2 dir = glm::vec2(playerPos) - glm::vec2(readPos());
	dir = normalize(dir);
	EchoLocation* pewpew = new EchoLocation(glm::vec2(readPos()), dir);
	pewpew->setVisitor();
	map->findNewHome(pewpew);
	delete pewpew;
}

int Bossbat::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec3 pos = readPos();
	if (pos.z < 0.0f)
	{
		moveTo(pos.x, pos.y, pos.z + 10.0f*deltaTime);
		if (pos.z >= -1.0f)
		{
			rotateTo(0, -(3.1415927f/2.0f)*deltaTime*10.0f, 0);
		}
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
		moveTo(pos.x, pos.y + speed*deltaTime);
		if (collidesWithWorld(map))
		{
			moveTo(pos.x, pos.y - speed*deltaTime);
		}
		spawnBat(map, deltaTime);
		if (invulnTimer < FLT_EPSILON)
		{
			chargePos = readPos();
			spawnsLeft = 3;
		}
	}
	else if (isShootingEchos)
	{
		if (spawnsLeft == 0)
		{
			echoTimer = 8.0f;
			spawnsLeft = 3;
			isShootingEchos = false;
		}
		else if (echoTimer < FLT_EPSILON)
		{
			echoLocation(map, playerPos);
			spawnsLeft--;
			echoTimer = 0.5f;
		}
		else
			echoTimer -= 1.0f*deltaTime;
	}
	else
	{
		echoTimer -= 1.0f*deltaTime;
		if (echoTimer < FLT_EPSILON)
			isShootingEchos = true;
		if (!charging)
		{
			if (pos.y < initPos.y - 0.5f)
			{
				moveTo(pos.x, pos.y + speed*deltaTime);
				if (collidesWithWorld(map))
				{
					moveTo(pos.x, pos.y - speed*deltaTime);
				}
			}
			else if (pos.y > initPos.y + 0.5f)
			{
				moveTo(pos.x, pos.y - speed*deltaTime);
				if (collidesWithWorld(map))
				{
					moveTo(pos.x, pos.y + speed*deltaTime);
				}
			}
		}
		pos = readPos();
		if (!charging)
		{
			if (facingRight)
			{
				if (movementScale < -1.0f)
					slow = true;
				else if (movementScale > 1.0f)
					slow = true;
				else
					slow = false;

				if (slow)
					moveTo(pos.x + speed*deltaTime * 0.5f, pos.y);
				if (!slow)
					moveTo(pos.x + speed*deltaTime, pos.y);
				movementScale += 1.0f*deltaTime;

				if (collidesWithWorld(map))
				{
					movementScale -= 1.0f*deltaTime;
					if (slow)
					{
						moveTo(pos.x - speed*deltaTime * 0.5f, pos.y);
						facingRight = false;
					}
					else
					{
						moveTo(pos.x - speed*deltaTime, pos.y);
						facingRight = false;
					}
				}

				if (movementScale > 1.5f)
					facingRight = false;
				if (!facingRight)
					rotateTo(0, 3.1415927f, 0);
			}
			else
			{
				if (movementScale < -1.0f)
					slow = true;
				else if (movementScale > 1.0f)
					slow = true;
				else
					slow = false;

				if (slow)
					moveTo(pos.x - speed*deltaTime * 0.5f, pos.y);
				if (!slow)
					moveTo(pos.x - speed*deltaTime, pos.y);
				movementScale -= 1.0f*deltaTime;

				if (collidesWithWorld(map))
				{
					movementScale += 1.0f*deltaTime;
					if (slow)
					{
						moveTo(pos.x + speed*deltaTime * 0.5f, pos.y);
						facingRight = true;
					}
					else
					{
						moveTo(pos.x + speed*deltaTime, pos.y);
						facingRight = true;
					}
				}

				if (movementScale < -1.5f)
					facingRight = true;
				if (facingRight)
					rotateTo(0, 3.1415927f, 0);
			}
		}
		else
		{
			chargeTimer -= 1.0f*deltaTime;
			float distX = returnPos.x - chargePos.x;
			float distY = returnPos.y - chargePos.y;
			moveTo(pos.x - speed*deltaTime*distX / 3, pos.y - speed*deltaTime*distY / 3);
			if (collidesWithWorld(map) || chargeTimer < FLT_EPSILON)
			{
				moveTo(pos.x + speed*deltaTime*distX / 3, pos.y + speed*deltaTime*distY / 3);
				charging = false;
				chargeTimer = 4.0f;
			}
		}

		if (!charging)
		{
			chargeTimer -= 1.0f*deltaTime;
			if (chargeTimer < FLT_EPSILON)
			{
				charging = true;
				chargePos = playerPos;
				returnPos = readPos();
				Audio::getAudio().playSoundAtPos(SoundID::boss_bat_attack, readPos(), audibleDistance + 2, false);//boss_bat_attack
				chargeTimer = 0.8f;
				if (chargePos.x < returnPos.x)
				{
					if (facingRight)
					{
						hasTurned = true;
						rotateTo(0, 3.1415927f, 0);
					}
				}
				else if (chargePos.x > returnPos.x)
				{
					if (!facingRight)
					{
						hasTurned = true;
						rotateTo(0, 3.1415927f, 0);
					}
				}
			}
			else if (chargeTimer > 3.5f)
			{
				float distX = chargePos.x - returnPos.x;
				float distY = chargePos.y - returnPos.y;
				moveTo(pos.x - speed*deltaTime*distX / 5, pos.y - speed*deltaTime*distY / 5);
				if (hasTurned)
				{
					hasTurned = false;
					rotateTo(0, 3.1415927f, 0);
				}
				if (collidesWithWorld(map))
				{
					moveTo(pos.x + speed*deltaTime*distX / 5, pos.y + speed*deltaTime*distY / 5);
					charging = false;
					chargeTimer = 3.5f;
				}
			}
		}
	}
	hurtRect->update();
	return 0;
}

bool Bossbat::isInitiated()
{
	return isInit;
}

void Bossbat::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		spawnsLeft = 3 + GameConfig::get().configDifficulty;
		batTimer = 0.6f;
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

bool Bossbat::isBlinking()
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

glm::vec2 Bossbat::plsGiveBossPos()
{
	return initPos;
}