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
	if (facingRight)
	{
		if (movementScale < 50.0f)
		{
			translate(3.0f*deltaTime, 0.0);
			movementScale += 0.05f;
		}
		else
		{
			facingRight = false;
		}
	}
	else
	{
		if (movementScale > -50.0f)
		{
			translate(-3.0f*deltaTime, 0.0);
			movementScale -= 0.05f;
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