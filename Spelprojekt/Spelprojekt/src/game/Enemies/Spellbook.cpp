#include "Spellbook.h"
#include "../map.h"
#include "Ghost.h"
#include "ArcaneMissile.h"

Spellbook::Spellbook(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = EnemyID::cube;
	health = 3;
	speed = 1.0f;
	audibleDistance = 4.5f;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

Spellbook::Spellbook(Spellbook* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	initPos = copy->initPos;
	glm::vec3 pos = copy->readPos();
	moveTo(pos.x, pos.y);
	alive = true;
	facingRight = copy->facingRight;
	contentIndex = EnemyID::cube;
	health = 3;
	speed = 1.0f;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

void Spellbook::init()
{
	worldMat = glm::mat4(1);
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 3;
	collideRect->update();
}

int Spellbook::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec2 pos = glm::vec2(readPos().x, readPos().y);
	glm::vec2 pPos = glm::vec2(playerPos.x, playerPos.y);

	if (pos.x < pPos.x && !facingRight)
	{
		rotateTo(0, 3.141592654f, 0);
		facingRight = true;
	}
	else if (pPos.x < pos.x && facingRight)
	{
		rotateTo(0, 3.141592654f, 0);
		facingRight = false;
	}

	// movement
	if (yMovement == 1)
	{
		moveTo(pos.x, pos.y + speed*deltaTime);

		if (pos.y > initPos.y + 1.0f || collidesWithWorld(map))
		{
			yMovement = -1;
		}
	}
	if (yMovement == -1)
	{
		moveTo(pos.x, pos.y - speed*deltaTime);

		if (pos.y < initPos.y - 1.0f || collidesWithWorld(map))
		{
			yMovement = 1;
		}
	}

	// spells
	if (invulnTimer < FLT_EPSILON)
	{
		if (spellCooldown <= 0)
		{
			glm::vec2 dist = pos - pPos;
			float distance = sqrt(dist.x*dist.x + dist.y*dist.y);

			if (distance < 10.0f)
			{
				castSpell(map, playerPos);
			}
		}
		else
		{
			spellCooldown -= 1.0f*deltaTime;
		}
	}
	else
	{
		pos = glm::vec2(readPos());
		invulnTimer -= 1.0f*deltaTime;
		rotateTo(0, 4*3.141592654f*deltaTime, 0);
		if (invulnTimer < FLT_EPSILON)
		{
			worldMat = glm::mat4(1);
			moveTo(pos.x, pos.y);
			if (!facingRight)
				rotateTo(0, 3.141592654f, 0);
			spellArcaneMissile(map, playerPos);
			spellCooldown = 2.0f;
		}
	}
	return 0;
}

void Spellbook::castSpell(Map* map, glm::vec3 playerPos)
{
	int spell = rand() % 4;

	if (spell == 0)
		spellSummonGhost(map, playerPos);
	else
		spellArcaneMissile(map, playerPos);
	
	spellCooldown = 3.0f;
}

void Spellbook::spellSummonGhost(Map* map, glm::vec3 playerPos)
{
	//test code
	if (minionCount < minionsMax)
	{
		Debug::DebugOutput("Casting spell Summon Ghost.\n");
		glm::vec3 pos = readPos();
		Ghost* summon = new Ghost(glm::vec2(pos));
		summon->setVisitor();
		map->findNewHome(summon);
		delete summon;
		Audio::getAudio().playSoundAtPos(SoundID::enemy_tome_spellcast, pos, audibleDistance, false);

		minionCount++;
	}
	else
		Debug::DebugOutput("Failed to cast spell Summon Ghost.\n");
}

void Spellbook::spellArcaneMissile(Map* map, glm::vec3 playerPos)
{
	Debug::DebugOutput("Casting spell Arcane Missile.\n");
	glm::vec3 pos = readPos();
	ArcaneMissile* pewpew = new ArcaneMissile(glm::vec2(pos));
	glm::vec2 dir = normalize(glm::vec2(playerPos) - glm::vec2(readPos()));
	pewpew->setDirection(dir);
	map->findNewHome(pewpew);
	delete pewpew;
	Audio::getAudio().playSoundAtPos(SoundID::enemy_tome_spellcast, pos, audibleDistance, false);
}

void Spellbook::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health <= 0)
		{
			alive = false;
			Audio::getAudio().playSoundAtPos(SoundID::enemy_tome_death, readPos(), audibleDistance, false);
		}
		else
			Audio::getAudio().playSoundAtPos(SoundID::enemy_tome_hurt, readPos(), audibleDistance, false); //spellbook_death (currently same as ghost)

		invulnTimer = 1.0f;
	}
	//	//else
	//		//audio(bookhurt)

	//	invulnTimer = 0.6f;
	//}
}