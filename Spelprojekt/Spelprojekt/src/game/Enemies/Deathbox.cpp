#include "Deathbox.h"
#include "../mapChunk.h"

Deathbox::Deathbox(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = WorldID::box;
	health = 0;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2);
}

void Deathbox::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;

	collideRect->update();
}

int Deathbox::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	return 0;
}

void Deathbox::hit(int damage, bool playerRightOfEnemy)
{
}