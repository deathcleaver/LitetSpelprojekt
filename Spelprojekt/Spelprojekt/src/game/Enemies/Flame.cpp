#include "Flame.h"
#include "../mapChunk.h"

Flame::~Flame()
{
	delete myLight;
}

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

	myLight = new Light();
	myLight->posX = initPos.x; myLight->posY = initPos.y; myLight->posZ = 0.0;
	myLight->r = 1.0; myLight->g = 0.0; myLight->b = 0.0f; myLight->intensity = 1.0; myLight->distance = 10.0;
	myLight->volume = 1;

	maxInt = 2.0f;
	minInt = 0.5f;
	increase = 0.01;
}

Flame::Flame(Flame* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	glm::vec3 pos = copy->readPos();
	initPos = glm::vec2(pos.x, pos.y);
	moveTo(pos.x, pos.y);
	alive = true;
	fading = true;
	copy->fading = false;
	facingRight = copy->facingRight;
	contentIndex = 2;
	health = copy->health;
	speed = copy->speed;
	invulnTimer = copy->invulnTimer;
	flying = copy->flying;

	myLight = new Light();

	maxInt = copy->maxInt;
	minInt = copy->minInt;
	increase = copy->increase;

	myLight->posX = copy->myLight->posX; myLight->posY = copy->myLight->posY; myLight->posZ = copy->myLight->posZ;
	myLight->r = 1.0; myLight->g = 0.0; myLight->b = 0.0f; myLight->intensity = 1.0; myLight->distance = 10.0;

	myLight->volume = 1;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.8f, 0.8f);
}

void Flame::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	fading = true;
	health = 3;
	invulnTimer = 0.0f;
	speed = glm::vec2(2.0f, 0.0);
	myLight->intensity = 1.0;
	myLight->volume = 1;
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

	glm::vec3 myPos = readPos();
	myLight->posX = myPos.x;
	myLight->posY = myPos.y;

	myLight->intensity += increase;
	if (myLight->intensity > maxInt || myLight->intensity < minInt)
	{
		increase = -increase;
	}

	return 0;
}

bool Flame::isFading()
{
	return fading;
}

void Flame::fade()
{
	myLight->intensity -= abs(increase);
	if (myLight->intensity < 0.0f)
		fading = false;
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