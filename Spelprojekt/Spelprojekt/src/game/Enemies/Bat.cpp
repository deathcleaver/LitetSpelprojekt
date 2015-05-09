#include "Bat.h"
#include "../map.h"

Bat::Bat(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = 1;
	speed = 4.0f;
	slow = false;
	audibleDistance = 2.5f;

	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

Bat::Bat(Bat* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	initPos = copy->initPos;
	glm::vec3 pos = copy->readPos();
	moveTo(pos.x, pos.y);
	alive = true;
	facingRight = copy->facingRight;
	contentIndex = 1;
	health = 1;
	speed = 4.0f;
	slow = copy->slow;

	movementScale = copy->movementScale;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

void Bat::init()
{
	moveTo(initPos.x, initPos.y);
	movementScale = 0.0f;
	if (!facingRight)
		rotateTo(0, 3.1415927f, 0);
	facingRight = true;
	alive = true;
	health = 1;
	slow = false;

	collideRect->update();
}

int Bat::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec3 pos = readPos();

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
	return 0;
}

void Bat::hit(int damage, bool playerRightOfEnemy)
{
	health -= damage;
	if (health <= 0)
	{
		Audio::getAudio().playSoundAtPos(SoundID::boss_bat_death, readPos(), audibleDistance, false); //boss_bat_death
		alive = false;
	}
}

void Bat::setFacing(bool faceRight)
{
	if (facingRight != faceRight)
		rotateTo(0, 3.1415927f, 0);
	facingRight = faceRight;
}