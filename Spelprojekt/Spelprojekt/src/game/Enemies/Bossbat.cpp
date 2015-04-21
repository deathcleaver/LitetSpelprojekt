#include "Bossbat.h"
#include "../mapChunk.h"

Bossbat::Bossbat(glm::vec2 firstPos)
{
	scaleFactor(2, 2, 2);
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = 4;
	speed = 4.0f;
	slow = false;

	invulnTimer = 0.0f;
	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1.8, 2);
}

void Bossbat::init()
{
	moveTo(initPos.x, initPos.y);
	invulnTimer = 0.0f;
	movementScale = 0.0f;
	if (!facingRight)
		rotate(0, 3.1415927f, 0);
	facingRight = true;
	alive = true;
	health = 4;
	slow = false;
	collideRect->update();
}

int Bossbat::update(float deltaTime, MapChunk* chunk)
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
		movementScale += 1.0*deltaTime;

		if (collidesWithWorld(chunk))
		{
			movementScale -= 1.0*deltaTime;
			if (slow)
			{
				moveTo(pos.x - speed*deltaTime * 0.5, pos.y);
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
			rotate(0, 3.1415927f, 0);
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
		movementScale -= 1.0*deltaTime;

		if (collidesWithWorld(chunk))
		{
			movementScale += 1.0*deltaTime;
			if (slow)
			{
				moveTo(pos.x + speed*deltaTime * 0.5, pos.y);
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
			rotate(0, 3.1415927f, 0);
	}

	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0*deltaTime;
	}
	return 0;
}

void Bossbat::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health > 0)
		{
			invulnTimer = 1.0f;
			printf("Boss took damage \n");
		}
		else
			alive = false;
	}
}

bool Bossbat::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
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