#include "Spikes.h"
#include "../mapChunk.h"

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

int Spikes::update(float deltaTime, MapChunk* chunk)
{
	return 0;
}

void Spikes::hit(int damage)
{
}