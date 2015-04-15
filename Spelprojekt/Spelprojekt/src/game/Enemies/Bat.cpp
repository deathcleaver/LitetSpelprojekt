#include "Bat.h"

Bat::Bat(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = 1;

	movementScale = 0.0f;
}

void Bat::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
}

int Bat::update(float deltaTime)
{
	float sinMod = sin(movementScale*0.8);
	if (facingRight)
	{
		if (movementScale < -5.0f)
		{
			float yset = sin(sinMod)*deltaTime*2.0;
			translate(1.5f*deltaTime, yset);
			movementScale += 5.0f * deltaTime;
		}
		else if (movementScale < 5.0f)
		{
			float yset = sin(sinMod)*deltaTime*2.0;
			translate(3.0f*deltaTime, yset);
			movementScale += 5.0f * deltaTime;
		}
		else if (movementScale < 6.0f)
		{
			float yset = sin(sinMod)*deltaTime*2.0;
			translate(1.5f*deltaTime, yset);
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
			float yset = sin(sinMod)*deltaTime*2.0;
			translate(-1.5f*deltaTime, -yset);
			movementScale -= 5.0f * deltaTime;
		}
		else if (movementScale > -5.0f)
		{
			float yset = sin(sinMod)*deltaTime*2.0;
			translate(-3.0f*deltaTime, -yset);
			movementScale -= 5.0f * deltaTime;
		}
		else if (movementScale > -6.0f)
		{
			float yset = sin(sinMod)*deltaTime*2.0;
			translate(-1.5f*deltaTime, -yset);
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