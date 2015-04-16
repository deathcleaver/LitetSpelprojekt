#include "Bat.h"
#include "../mapChunk.h"

Bat::Bat(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = 1;
	speed = 3.0f;

	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

void Bat::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;

	collideRect->update();
}

int Bat::update(float deltaTime, MapChunk* chunk)
{
	float sinMod = 0.0f;// sin(movementScale*0.8);
	float yset = sinMod*deltaTime*2.0f;
	if (facingRight)
	{
		if (movementScale < -5.0f)
		{
			translate(speed*0.5f*deltaTime, yset*0.5f);
			if (collidesWithWorld(chunk))
			{
				translate(-speed*0.5f*deltaTime, -yset*0.5f);
				facingRight = false;
			}
			else
				movementScale += 5.0f * deltaTime;
		}
		else if (movementScale < 5.0f)
		{
			translate(speed*deltaTime, yset);
			if (collidesWithWorld(chunk))
			{
				translate(-speed*deltaTime, -yset);
				facingRight = false;
			}
			else
				movementScale += 5.0f * deltaTime;
		}
		else if (movementScale < 6.0f)
		{
			translate(speed*0.5f*deltaTime, yset*0.5f);
			if (collidesWithWorld(chunk))
			{
				translate(-speed*0.5f*deltaTime, -yset*0.5f);
				facingRight = false;
			}
			else
				movementScale += 5.0f * deltaTime;
		}
		else
		{
			facingRight = false;
		}
	}
	else
	{
		if (movementScale > 5.0f)
		{
			translate(-speed*0.5f*deltaTime, -yset*0.5f);
			if (collidesWithWorld(chunk))
			{
				translate(speed*0.5f*deltaTime, yset*0.5f);
				facingRight = true;
			}
			else
				movementScale -= 5.0f * deltaTime;
		}
		else if (movementScale > -5.0f)
		{
			translate(-speed*deltaTime, -yset);
			if (collidesWithWorld(chunk))
			{
				translate(speed*deltaTime, yset);
				facingRight = true;
			}
			else
				movementScale -= 5.0f * deltaTime;
		}
		else if (movementScale > -6.0f)
		{
			translate(-speed*0.5f*deltaTime, -yset*0.5f);
			if (collidesWithWorld(chunk))
			{
				translate(speed*0.5f*deltaTime, yset*0.5f);
				facingRight = true;
			}
			else
				movementScale -= 5.0f * deltaTime;
		}
		else
		{
			facingRight = true;
		}
	}
	return 0;
}

void Bat::hit(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		alive = false;
	}
}

bool Bat::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}