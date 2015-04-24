#include "Cube.h"
#include "../mapChunk.h"

Cube::Cube(glm::vec2 firstPos)
{
	scaleFactor(1.8, 1.8, 1.8);
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 2;
	health = 4;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1.8, 1.8);

	speed = glm::vec2(0,0);
	maxSpeed = glm::vec2(12, 30);
	acc = 0.3f;
	invulnTimer = 0.0f;
}

Cube::Cube(Cube* copy)
{
	scaleFactor(1.8, 1.8, 1.8);
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	glm::vec3 copyPos = copy->readPos();
	initPos = glm::vec2(copyPos.x, copyPos.y);
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = copy->facingRight;
	contentIndex = 2;
	health = copy->health;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1.8, 1.8);

	speed = copy->speed;
	maxSpeed = glm::vec2(12, 30);
	acc = 0.3f;
	invulnTimer = copy->invulnTimer;
}

void Cube::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 4;
	collideRect->update();
}

int Cube::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos)
{
	glm::vec3 myPos = readPos();
	if (playerPos.x < myPos.x)
	{
		speed.x -= acc;
		if (speed.x < 0.0)
			facingRight = false;
		if (speed.x < -maxSpeed.x && invulnTimer < FLT_EPSILON)
			speed.x = -maxSpeed.x;
		moveTo(myPos.x + speed.x*deltaTime, myPos.y);
		if (collidesWithWorld(chunk))
		{
			moveTo(myPos.x - speed.x*deltaTime, myPos.y);
			speed.x = 0;
		}
	}
	else if (playerPos.x > myPos.x)
	{
		speed.x += acc;
		if (speed.x > 0.0)
			facingRight = true;
		if (speed.x > maxSpeed.x && invulnTimer < FLT_EPSILON)
			speed.x = maxSpeed.x;
		moveTo(myPos.x + speed.x*deltaTime, myPos.y);
		if (collidesWithWorld(chunk))
		{
			moveTo(myPos.x - speed.x*deltaTime, myPos.y);
			speed.x = 0;
		}
	}
	myPos = readPos();
	speed.y += 1.0f;
	if (speed.y > maxSpeed.y)
		speed.y = maxSpeed.y;
	moveTo(myPos.x, myPos.y - speed.y*deltaTime);
	if (collidesWithWorld(chunk))
	{
		moveTo(myPos.x, myPos.y + speed.y*deltaTime);
		speed.y = 0;
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
	}
	return 0;
}

void Cube::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (playerRightOfEnemy)
		{
			speed.x = -16.0f;
			speed.y = 20.0f;
		}
		else
		{
			speed.x = 16.0f;
			speed.y = 20.0f;
		}
		if (health <= 0)
		{
			alive = false;
			//Spawna nya slimes
		}
		invulnTimer = 0.6f;
	}

}

bool Cube::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}