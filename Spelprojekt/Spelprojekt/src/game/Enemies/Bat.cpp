#include "Bat.h"

Bat::Bat(glm::vec2 firstPos)
{
	initPos = firstPos;
	alive = true;
	facingRight = true;
	contentIndex = 11;
	wingsUpIndex = 12;
	health = 1;
	interpolation = 0.0f;
}

void Bat::init()
{
	translate(initPos.x, initPos.y);
}

int Bat::update(float deltaTime)
{
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

void Bat::getContentIndices(int &state1, int &state2, float &interpol)
{
	state1 = contentIndex;
	state2 = wingsUpIndex;
	interpol = interpolation;
}
