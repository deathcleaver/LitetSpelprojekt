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
	speed = 1.0f;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);
}

void Flame::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;

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
		}
		else
		{
			translate(-1.0f, 1.0f);
			translate(speed*deltaTime, 0.0f);
			if (collidesWithWorld(chunk))
			{
				translate(-speed*deltaTime, 0.0f);
				facingRight = false;
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
		}
		else
		{
			translate(1.0f, 1.0f);
			translate(-speed*deltaTime, 0.0f);
			if (collidesWithWorld(chunk))
			{
				translate(speed*deltaTime, 0.0f);
				facingRight = true;
			}
		}
	}
	collideRect->update();
	return 0;
}

void Flame::hit(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		alive = false;
	}
}

bool Flame::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}