#include "Flame.h"
#include "../mapChunk.h"

Flame::Flame(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 2;
	health = 3;
	speed = 2.0f;
	invulnTimer = 0.0f;
	hasBounced = true;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);
}

void Flame::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 3;
	invulnTimer = 0.0f;
	hasBounced = true;
	speed = 2.0f;

	collideRect->update();
}

int Flame::update(float deltaTime, MapChunk* chunk)
{
	if (facingRight)
	{
		translate(1.0f, -1.0f);
		if (!collidesWithWorld(chunk))
		{
			facingRight = false;
			translate(-1.0f, 1.0f);
			if (invulnTimer > FLT_EPSILON)
			{
				speed = 2.0f;
				hasBounced = true;
			}
		}
		else
		{
			translate(-1.0f, 1.0f);
			translate(speed*deltaTime, 0.0f);
			if (collidesWithWorld(chunk))
			{
				translate(-speed*deltaTime, 0.0f);
				facingRight = false;

				if (invulnTimer > FLT_EPSILON)
				{
					speed = 2.0f;
					hasBounced = true;
				}
			}
		}
	}
	else
	{
		translate(-1.0f, -1.0f);
		if (!collidesWithWorld(chunk))
		{
			facingRight = true;
			translate(1.0f, 1.0f);
			if (invulnTimer > FLT_EPSILON)
			{
				speed = 2.0f;
				hasBounced = true;
			}
		}
		else
		{
			translate(1.0f, 1.0f);
			translate(-speed*deltaTime, 0.0f);
			if (collidesWithWorld(chunk))
			{
				translate(speed*deltaTime, 0.0f);
				facingRight = true;
				if (invulnTimer > FLT_EPSILON)
				{
					speed = 2.0f;
					hasBounced = true;
				}
			}
		}
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
		if (invulnTimer < FLT_EPSILON)
		{
			speed = 2.0f;
			if (!hasBounced)
			{
				if (facingRight)
					facingRight = false;
				else
					facingRight = true;
			}
		}
	}
	collideRect->update();
	return 0;
}

void Flame::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		speed = 4.0f;
		health -= damage;
		if (health <= 0)
		{
			alive = false;
		}
		invulnTimer = 0.7f;
		if (playerRightOfEnemy)
		{
			if (facingRight)
			{
				facingRight = false;
				hasBounced = false;
			}
		}
		else
		{
			if (!facingRight)
			{
				facingRight = true;
				hasBounced = false;
			}
		}
	}
}

bool Flame::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}