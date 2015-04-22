#include "Flame.h"
#include "../mapChunk.h"

Flame::Flame(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 2;
	health = 3;
	speed = glm::vec2(2.0f, 0.0);
	invulnTimer = 0.0f;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);
}

Flame::Flame(Flame* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	glm::vec3 pos = copy->readPos();
	initPos = glm::vec2(pos.x, pos.y);
	moveTo(pos.x, pos.y);
	alive = true;
	facingRight = copy->facingRight;
	contentIndex = 2;
	health = copy->health;
	speed = copy->speed;
	invulnTimer = copy->invulnTimer;
	flying = copy->flying;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.8f, 0.8f);
}

void Flame::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 3;
	invulnTimer = 0.0f;
	speed = glm::vec2(2.0f, 0.0);

	collideRect->update();
}

int Flame::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos)
{
	if (!flying)
	{
		if (facingRight)
		{
			translate(1.0f, -1.0f);
			if (!collidesWithWorld(chunk))
			{
				facingRight = false;
				translate(-1.0f, 1.0f);
			}
			else
			{
				translate(-1.0f, 1.0f);
				translate(speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(chunk))
				{
					translate(-speed.x*deltaTime, 0.0f);
					facingRight = false;
				}
			}
		}
		else
		{
			translate(-1.0f, -1.0f);
			if (!collidesWithWorld(chunk))
			{
				facingRight = true;
				translate(1.0f, 1.0f);
			}
			else
			{
				translate(1.0f, 1.0f);
				translate(-speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(chunk))
				{
					translate(speed.x*deltaTime, 0.0f);
					facingRight = true;
				}
			}
		}
	}
	else
	{
		if (facingRight)
			translate(speed.x*deltaTime, 0);
		else
			translate(-speed.x*deltaTime, 0);
		if (collidesWithWorld(chunk))
		{
			if (facingRight)
				translate(-speed.x*deltaTime, 0);
			else
				translate(speed.x*deltaTime, 0);
			speed.x = 0;
		}

		speed.y -= 1.0f;
		if (speed.y < -20.0f)
		{
			speed.y = -20.0f;
		}
		translate(0, speed.y*deltaTime);
		if (collidesWithWorld(chunk))
		{
			if (speed.y > 0)
			{
				translate(0, -speed.y*deltaTime);
				speed.y = 0;
			}
			else
			{
				translate(0, -speed.y*deltaTime);
				translate(0, speed.y*deltaTime*0.5);
				if (collidesWithWorld(chunk))
				{
					translate(0, -speed.y*deltaTime*0.5);
					translate(0, speed.y*deltaTime*0.25);
					if (collidesWithWorld(chunk))
						translate(0, -speed.y*deltaTime*0.25);
				}
				speed.x = 2.0f;
				speed.y = 0.0f;
				flying = false;
			}
		}
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
	}
	collideRect->update();
	return 0;
}

void Flame::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		speed.x = 12.0f;
		speed.y = 15.0f;
		flying = true;
		health -= damage;
		if (health <= 0)
		{
			alive = false;
		}
		invulnTimer = 0.6f;
		if (playerRightOfEnemy)
		{
			if (facingRight)
			{
				facingRight = false;
			}
		}
		else
		{
			if (!facingRight)
			{
				facingRight = true;
			}
		}
	}
}

bool Flame::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}