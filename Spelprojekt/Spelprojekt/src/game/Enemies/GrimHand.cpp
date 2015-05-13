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

	if (contentIndex == EnemyID::grimhand_left)
		neutralPos = glm::vec2(initPos.x + 9.0f, initPos.y - 2.0f);
	else
		neutralPos = glm::vec2(initPos.x - 9.0f, initPos.y - 2.0f);
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
			calcDir(neutralPos);
		}
	}
	else
	{
		if (contentIndex == EnemyID::grimhand_left)
		{
			leftHandState(deltaTime, map, playerPos);
		}
		else
		{
			rightHandState(deltaTime, map, playerPos);
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

void GrimHand::leftHandState(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec2 pos = glm::vec2(readPos());
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
			speed.x = 0;
			speed.y = 0;
			state = 1;
			calcDir(glm::vec2(pos.x - 2.0f, pos.y - 4.0f));
		}
	}
	else if (state == 1) //Hover, wait for player
	{
		if (!reachedDestination())
		{
			speed.x += 0.1f;
			if (speed.x > 3.0f)
				speed.x = 3.0f;
			speed.y += 0.1f;
			if (speed.y > 3.0f)
				speed.y = 3.0f;
			translate(speed.x*dirToFly.x*deltaTime, speed.y*dirToFly.y*deltaTime);
		}
		else
		{
			if (dirToFly.y < 0.0f)
				calcDir(glm::vec2(pos.x + 2.0f, pos.y + 4.0f));
			else
				calcDir(glm::vec2(pos.x - 2.0f, pos.y - 4.0f));
			speed.y = -speed.y;
			speed.x = -speed.x;
		}
		if (playerPos.x > initPos.x) //Player enters right side of arena
		{
			speed.x = 0;
			speed.y = 0;
			stateTimer = 1.5f;
			state = 2; //GIT VIOLENT
			calcDir(glm::vec2(playerPos));
		}
	}
	else if (state == 2) //Swing at player
	{
		speed.x += 0.8f;
		speed.y += 0.8f;
		if (speed.x > 22.0f)
			speed.x = 22.0f;
		if (speed.y > 22.0f)
			speed.y = 22.0f;
		if (stateTimer > 0.6f)
		{
			translate(dirToFly.x*speed.x*deltaTime, dirToFly.y*speed.y*deltaTime);
			stateTimer -= 1.0*deltaTime;
		}
		else
		{
			translate(-dirToFly.x*speed.x*deltaTime, -dirToFly.y*speed.y*deltaTime);
			stateTimer -= 1.0*deltaTime;
		}
		if (stateTimer < FLT_EPSILON)
		{
			state = 3;
			speed.x = speed.y = 0;
			calcDir(glm::vec2(neutralPos.x, playerPos.y));
			stateTimer = 0.0f;
			leftHandSwingCounter = 3;
		}
		if (playerPos.x < initPos.x - 1.0f) //-1 is an asshole buffer
		{
			state = -1;
			stateTimer = 0.0f;
			speed.x = speed.y = 0;
		}
	}
	else if (state == 3) //Swing horizontally three times
	{
		if (stateTimer < FLT_EPSILON && leftHandSwingCounter == 3)
		{
			if (!reachedDestination())
			{
				speed.x += 1.1f;
				speed.y += 1.1f;
				if (speed.x > 22.0f)
					speed.x = 22.0f;
				if (speed.y > 22.0f)
					speed.y = 22.0f;
				translate(dirToFly.x*speed.x*deltaTime, dirToFly.y*speed.y*deltaTime);
			}
			else
			{
				pos = glm::vec2(readPos());
				speed.x = speed.y = 0;
				stateTimer = 1.0;
				calcDir(glm::vec2(pos.x-1.0f, pos.y));
			}
		}
		else
		{
			stateTimer -= 1.0*deltaTime;
			speed.x += 1.1f;
			speed.y += 1.1f;
			if (speed.x > 22.0f)
				speed.x = 22.0f;
			if (speed.y > 22.0f)
				speed.y = 22.0f;
			if (stateTimer > 0.5f)
				translate(dirToFly.x*speed.x*deltaTime, dirToFly.y*speed.y*deltaTime);
			else
				translate(-dirToFly.x*speed.x*deltaTime, -dirToFly.y*speed.y*deltaTime);
			if (stateTimer < FLT_EPSILON)
			{
				stateTimer = 1.0f;
				leftHandSwingCounter--;
			}
			if (leftHandSwingCounter == 0)
			{
				speed.x = speed.y = 0;
				state = 4;
			}
		}
	}
}

void GrimHand::rightHandState(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec2 pos = glm::vec2(readPos());
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
	else if (state == 1)
	{
		if (dirToFly.y < 0.0f && playerPos.y > pos.y)
			speed.y = -speed.y;
		if (dirToFly.y > 0.0f && playerPos.y < pos.y)
			speed.y = -speed.y;
		speed.y += 0.1f;
		if (speed.y > 6.0f)
			speed.y = 6.0f;
		calcDir(glm::vec2(pos.x, playerPos.y));
		translate(0, speed.y*dirToFly.y*deltaTime);
		if (playerPos.x < initPos.x) //Player on left side of arena
		{
			stateTimer -= 1.0*deltaTime;
			if (stateTimer < FLT_EPSILON)
			{
				state = 2; //GIT VIOLENT
				stateTimer = 0.9f;
				calcDir(glm::vec2(playerPos));
				speed.x = speed.y = 0;
			}
		}
	}
	else if (state == 2) //Follow player
	{
		stateTimer -= 1.0f*deltaTime;
		speed.x += 0.9f;
		speed.y += 0.9f;
		if (speed.x > 16.0f)
			speed.x = 16.0f;
		if (speed.y > 16.0f)
			speed.y = 16.0f;
		translate(speed.x*dirToFly.x*deltaTime, speed.y*dirToFly.y*deltaTime);
		if (stateTimer < FLT_EPSILON)
		{
			stateTimer = 0.9f;
			calcDir(glm::vec2(playerPos));
		}
		if (playerPos.x > initPos.x + 1.0f) //+1 is an asshole buffer
		{
			state = -1;
			stateTimer = 0.0f;
			speed.x = speed.y = 0;
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