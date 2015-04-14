#include "Spikes.h"

Spikes::Spikes(glm::vec2 firstPos)
{
	initPos = firstPos;
	alive = true;
	facingRight = true;
	contentIndex = 10;
}

void Spikes::init()
{
	translate(initPos.x, initPos.y);
}

int Spikes::update(float deltaTime)
{
	return 0;
}
