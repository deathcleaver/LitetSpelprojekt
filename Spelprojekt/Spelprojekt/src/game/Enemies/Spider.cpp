#include "spider.h"

Spider::Spider(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	contentIndex = 4;
	alive = true;
	jumpTimer = 0.0f;
	ceiling = true;
	jumping = false;
	facingRight = true;
	health = 2;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

Spider::Spider(Spider* copy)
{
	worldMat = copy->worldMat;
	initPos = glm::vec2(copy->readPos().x, copy->readPos().y);
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = copy->jumping;
	jumping = copy->jumping;
	jumpTimer = copy->jumpTimer;
	ceiling = copy->ceiling;
	health = copy->health;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);
}

Spider::~Spider()
{
}

void Spider::init()
{
	moveTo(initPos.x, initPos.y);
	alive = true;
	jumpTimer = 0.0f;
	ceiling = true;
	jumping = false;
	facingRight = true;
	health = 2;
	collideRect->update();
}

int Spider::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (ceiling)
	{
		if (facingRight)
		{
			translate(1.0f, -1.0f);
			if (!collidesWithWorld(map))
			{
				facingRight = false;
				translate(-1.0f, 1.0f);
			}
			else
			{
				translate(-1.0f, 1.0f);
				translate(speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(map))
				{
					translate(-speed.x*deltaTime, 0.0f);
					facingRight = false;
				}
			}
		}
		else
		{
			translate(-1.0f, -1.0f);
			if (!collidesWithWorld(map))
			{
				facingRight = true;
				translate(1.0f, 1.0f);
			}
			else
			{
				translate(1.0f, 1.0f);
				translate(-speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(map))
				{
					translate(speed.x*deltaTime, 0.0f);
					facingRight = true;
				}
			}
		}
	}
	return 0;
}

void Spider::hit(int damage, bool playerRightOfEnemy)
{
	health = damage;
	if (health <= 0)
	{
		alive = false;
	}
}
