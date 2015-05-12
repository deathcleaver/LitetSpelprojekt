#include "Bossspider.h"
#include "../mapChunk.h"
#include "../map.h"
#include "Webshot.h"

Bossspider::Bossspider(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::maexxna;
	health = 6;
	speed = glm::vec2(8.0f, 0.0f);
	speed.x += GameConfig::get().configDifficulty * 2; // + (0->2)
	audibleDistance = 10.0f;

	invulnTimer = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 3.1f, 2.8f);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 3.3f, 3.2f);

	currentMode = -1;
	webTimer = 0.0f;
	jumpTimer = -1.0f;
}

Bossspider::~Bossspider()
{
	delete hurtRect;
}

void Bossspider::init()
{
	if (!isInit)
	{
		worldMat = glm::mat4(1);
		rotateTo(3.14159265f, 0, 0);
		isInit = true;
		moveTo(initPos.x, initPos.y);
		invulnTimer = 0.0f;
		facingRight = true;
		alive = true;
		health = 6;
		collideRect->update();
		hurtRect->update();

		speed = glm::vec2(8.0f, 0.0f);
		currentMode = -1;
		webTimer = 2.0f;
		websToShoot = 0;
		jumpTimer = -1.0f;
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

void Bossspider::howDoIShotWeb(glm::vec3 playerPos, Map* map)
{
	Audio::getAudio().playSoundAtPos(SoundID::boss_spider_attack, readPos(), audibleDistance, false);
	Webshot* pewpew = new Webshot(glm::vec2(readPos()));
	pewpew->setVisitor();
	glm::vec2 dir = glm::vec2(playerPos) - glm::vec2(readPos());
	dir.x = dir.x + (rand() % 5 - 2);
	dir.y = dir.y + (rand() % 5 - 2);
	dir = normalize(dir);
	pewpew->setDirection(dir);
	map->findNewHome(pewpew);
	delete pewpew;
	websToShoot--;
}

int Bossspider::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (invulnTimer > 0.0f)
		invulnTimer -= 1.0f*deltaTime;
	if (currentMode == -1) //Spawning
	{
		webTimer -= 1.0f*deltaTime;
		if (webTimer < FLT_EPSILON)
		{
			Audio::getAudio().playSoundAtPos(SoundID::boss_spider_spawn, readPos(), audibleDistance, false);
			currentMode = 0;
			rotateTo(-3.14159265f, 0, 0);
		}
	}
	else if (currentMode == 0) //Dropping from ceiling
	{
		speed.y -= 4.0;
		if (speed.y < -25)
			speed.y = -25;
		translate(0, speed.y*deltaTime);
		if (collidesWithWorld(map))
		{
			translate(0, -speed.y*deltaTime);
			speed.y = 0;
			currentMode = 1;
			jumpTimer = 7.0f;
			Debug::DebugOutput("Mode switch to 1\n");

		}
	}
	else if (currentMode == 1) //Rolling around at the speed of sound, got places to go, gotta follow my rainbow
	{
		translate(speed.x*deltaTime, 0);
		if (collidesWithWorld(map))
		{
			translate(-speed.x*deltaTime, 0);
			glm::vec3 pos = readPos();
			if (facingRight)
			{
				facingRight = false;
				worldMat = glm::mat4(1);
				moveTo(pos.x, pos.y);
				rotateTo(0, 3.14159265f, 0);
			}
			else
			{
				facingRight = true;
				worldMat = glm::mat4(1);
				moveTo(pos.x, pos.y);
			}
			speed.x = -speed.x;
		}
		jumpTimer -= 1.0f*deltaTime;
		if (jumpTimer < FLT_EPSILON)
		{
			speed.y = 25.0f;
			currentMode = 2;
			Debug::DebugOutput("Modeswitch to 2\n");
		}
	}
	else if (currentMode == 2) //Jumping back up
	{
		speed.y -= 0.2f;
		translate(0, speed.y*deltaTime);
		if (collidesWithWorld(map))
		{
			if (speed.y >= 0)
			{
				rotateTo(3.14159265f, 0, 0);
				translate(0, -speed.y*deltaTime);
				currentMode = 3;
				webTimer = 1.0f;
				if (health > 4)
					websToShoot = 1 + GameConfig::get().configDifficulty * 2;
				else if (health > 2)
					websToShoot = 2 + GameConfig::get().configDifficulty * 2;
				else
					websToShoot = 3 + GameConfig::get().configDifficulty * 2;
				Debug::DebugOutput("Modeswitch to 3\n");
			}
			else
			{
				translate(0, -speed.y*deltaTime);
				currentMode = 1;
				Debug::DebugOutput("Landed on ground after jump. Modeswitch to 1\n");
				jumpTimer = 5.0f;
			}
			speed.y = 0;
		}
	}
	else if (currentMode == 3) //Spiderman, spiderman, shoots whatever a spider can
	{
		webTimer -= 1.0f*deltaTime;
		if (webTimer < FLT_EPSILON && websToShoot == 0)
		{
			rotateTo(-3.14159265f, 0, 0);
			currentMode = 0;
			Debug::DebugOutput("Modeswitch to 0\n");
		}
		else
		{
			if (webTimer < FLT_EPSILON)
			{
				howDoIShotWeb(playerPos, map);
				webTimer = 0.5f;
			}
		}
	}
	hurtRect->update();
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
			invulnTimer = 1.2f;
			Debug::DebugOutput("Boss took damage \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_spider_hurt, readPos(), audibleDistance, false);//boss_bat_hurt
			if (currentMode == 1)
			{
				jumpTimer = 1.0f;
				speed.y = 25.0f;
			}
		}
		else if (alive == true)
		{
			alive = false;
			Debug::DebugOutput("Boss is dead \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_spider_death, readPos(), audibleDistance, false);//boss_bat_death
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