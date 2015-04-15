#include "Spikes.h"

Spikes::Spikes(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 0;
	health = 0;
}

void Spikes::init()
{
	translate(initPos.x, initPos.y);
}

int Spikes::update(float deltaTime)
{
	return 0;
}

void Spikes::hit(int damage)
{
}

void Spikes::getContentIndices(int &state1, int &state2, float &interpol)
{
	state1 = state2 = contentIndex;
	interpol = 0.0f;
}