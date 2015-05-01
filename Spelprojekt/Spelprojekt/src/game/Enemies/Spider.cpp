#include "spider.h"

Spider::Spider(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	contentIndex = 4;
	alive = true;
	jumpTimer = 3.0f;
	ceiling = true;
	jumping = false;
	facingRight = true;
	health = 2;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);
	speed = glm::vec2(1, 0.0);

	invulnTimer = 0.0f;
}

Spider::Spider(Spider* copy)
{
	visitor = copy->visitor;
	contentIndex = 4;
	worldMat = copy->worldMat;
	initPos = glm::vec2(copy->readPos().x, copy->readPos().y);
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = copy->jumping;
	jumping = copy->jumping;
	jumpTimer = copy->jumpTimer;
	ceiling = copy->ceiling;
	health = copy->health;
	speed = copy->speed;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);

	invulnTimer = copy->invulnTimer;
}

Spider::~Spider()
{
}

void Spider::init()
{
	moveTo(initPos.x, initPos.y);
	alive = true;
	jumpTimer = 3.0f;
	ceiling = true;
	jumping = false;
	facingRight = true;
	health = 2;
	speed = glm::vec2(1, 0.0);
	collideRect->update();

	invulnTimer = 0.0f;
}

int Spider::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (ceiling)
	{
		if (facingRight)
		{
			translate(1.0f, 1.0f);
			if (!collidesWithWorld(map))
			{
				facingRight = false;
				translate(-1.0f, -1.0f);
			}
			else
			{
				translate(-1.0f, -1.0f);
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
			translate(-1.0f, 1.0f);
			if (!collidesWithWorld(map))
			{
				facingRight = true;
				translate(1.0f, -1.0f);
			}
			else
			{
				translate(1.0f, -1.0f);
				translate(-speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(map))
				{
					translate(speed.x*deltaTime, 0.0f);
					facingRight = true;
				}
			}
		}

		glm::vec3 pos = readPos();
		if (playerPos.y < pos.y && playerPos.x - pos.x < 2 && playerPos.x - pos.x > -2)
		{
			printf("DROP IT!\n");
			ceiling = false;
			jumping = true;
			speed.x = 0.0f;
		}
	}
	else
	{
		if (jumping)
		{
			speed.y -= 1.0f;
			if (speed.y < -25.0f)
				speed.y = -25.0f;
			translate(0, speed.y*deltaTime);
			if (collidesWithWorld(map))
			{
				glm::vec3 pos = readPos();
				translate(0, -speed.y*deltaTime);
				if (speed.y < 0.0f)
				{
					jumpTimer = 3.0f;
					jumping = false;
					speed.x = 4.0f;
					if (playerPos.x < pos.x && facingRight)
						facingRight = false;
					if (playerPos.x > pos.x && !facingRight)
						facingRight = true;
				}
				speed.y = 0.0f;
			}
			if (facingRight)
			{
				translate(speed.x*deltaTime, 0);
				if (collidesWithWorld(map))
				{
					translate(-speed.x*deltaTime, 0);
					speed.x = 0.0f;
				}
			}
			else
			{
				translate(-speed.x*deltaTime, 0);
				if (collidesWithWorld(map))
				{
					translate(speed.x*deltaTime, 0);
					speed.x = 0.0f;
				}
			}
		}
		else
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
			jumpTimer -= 1.0f*deltaTime;
			if (jumpTimer < FLT_EPSILON)
			{
				glm::vec3 pos = readPos();
				if (facingRight && playerPos.x < pos.x)
					facingRight = false;
				else if (!facingRight && playerPos.x > pos.x)
					facingRight = true;
				if (playerPos.x - pos.x > 7 || playerPos.x - pos.x < -7)
					speed.x = 14.0;
				else
					speed.x = 8.0;
				speed.y = 14.0;
				jumping = true;
			}
		}
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
	}
	return 0;
}

void Spider::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		invulnTimer = 0.7f;
		health -= damage;
		if (health <= 0)
		{
			alive = false;
		}
		if (playerRightOfEnemy)
		{
			if (facingRight)
				facingRight = false;
		}
		else
		{
			if (!facingRight)
				facingRight = true;
		}
		speed.x = 12;
		speed.y = 15;
		jumping = true;
		if (ceiling)
			ceiling = false;
	}
}
