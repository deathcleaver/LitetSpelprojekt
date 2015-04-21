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
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

void Spikes::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;

	collideRect->update();
}

int Spikes::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos)
{
	return 0;
}

void Spikes::hit(int damage, bool playerRightOfEnemy)
{
}

string Spikes::isBoss()
{
	return "ChuckTesta";
}