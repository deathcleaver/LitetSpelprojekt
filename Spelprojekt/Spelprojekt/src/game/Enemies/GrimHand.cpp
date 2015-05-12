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

void GrimHand::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 2;

	speed.x = speed.y = 0;
	invulnTimer = 1.0f;
	stateTimer = 4.0f;
	state = -1;
	grimMode = 0;
	clapTimer = 10.0f;

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
			glm::vec2 pos = glm::vec2(readPos());
			if (contentIndex == EnemyID::grimhand_left)
				calcDir(glm::vec2(pos.x + 6.0f, pos.y - 2.0f));
			else
				calcDir(glm::vec2(pos.x - 6.0f, pos.y - 2.0f));
		}
	}
	else
	{
		if (contentIndex == EnemyID::grimhand_left)
		{
			leftHandState(state, deltaTime, map, playerPos);
		}
		else
		{
			rightHandState(state, deltaTime, map, playerPos);
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

void GrimHand::leftHandState(int state, float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (state == 0) //Fly out to right side of screen
	{
		if (!reachedDestination())
		{
			speed.x += 0.1f;
			speed.y += 0.1f;
			if (speed.x > 6.0f)
				speed.x = 6.0f;
			if (speed.y > 6.0f)
				speed.y = 6.0f;
			translate(speed.x*dirToFly.x*deltaTime, speed.y*dirToFly.y*deltaTime);
		}
		else
		{
			state = 1;
		}
	}
}

void GrimHand::rightHandState(int state, float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (state == 0) //Fly out to left side of screen
	{
		if (!reachedDestination())
		{
			speed.x += 0.1f;
			speed.y += 0.1f;
			if (speed.x > 6.0f)
				speed.x = 6.0f;
			if (speed.y > 6.0f)
				speed.y = 6.0f;
			translate(speed.x*dirToFly.x*deltaTime, speed.y*dirToFly.y*deltaTime);
		}
		else
		{
			state = 1;
		}
	}
}

void GrimHand::calcDir(glm::vec2 destination)
{
	glm::vec2 pos = glm::vec2(readPos());
	
	currentGoal = destination;
	dirToFly = currentGoal - pos;
	dirToFly = normalize(dirToFly);
}

bool GrimHand::reachedDestination()
{
	glm::vec3 pos = readPos();
	if (pos.x < currentGoal.x + 1.0f && pos.x > currentGoal.x - 1.0f &&
		pos.y < currentGoal.y + 1.0f && pos.y > currentGoal.y - 1.0f)
		return true;
	return false;
}