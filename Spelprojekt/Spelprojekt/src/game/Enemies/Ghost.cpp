#include "Ghost.h"
#include "../mapChunk.h"

Ghost::Ghost(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = 3;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);

	speed = 3.0f;
}

Ghost::Ghost(Ghost* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	initPos = glm::vec2(readPos());
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 1;
	health = copy->health;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);

	speed = 3.0f;
}

void Ghost::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 3;

	collideRect->update();
}

int Ghost::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (invulnTimer < FLT_EPSILON)
	{
		glm::vec2 pos = glm::vec2(readPos().x, readPos().y);
		glm::vec2 pPos = glm::vec2(playerPos.x, playerPos.y);
		glm::vec2 dist = pos - pPos;
		float distance = sqrt(dist.x*dist.x + dist.y*dist.y);

		if (distance < 10.0f)
		{
			glm::vec2 normDist = normalize(dist);
			float xSpeed = -normDist.x*speed;
			float ySpeed = -normDist.y*speed;
			moveTo(pos.x + xSpeed*deltaTime, pos.y + ySpeed*deltaTime);
		}
		collideRect->update();
	}
	else
		invulnTimer -= 1.0f*deltaTime;
	return 0;
}

void Ghost::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health <= 0)
			alive = false;
		invulnTimer = 2.0f;
	}
}

bool Ghost::isBlinking()
{
	if (invulnTimer > FLT_EPSILON)
		return true;
	return false;
}