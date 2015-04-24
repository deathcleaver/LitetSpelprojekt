#include "Bossbat.h"
#include "../mapChunk.h"
#include "../map.h"
#include "Bat.h"

Bossbat::Bossbat(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = 1;
	health = 4;
	speed = 4.0f;
	slow = false;

	invulnTimer = 0.0f;
	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2.5);
	batsToSpawn = 0;
	batTimer = 0.0f;
	charging = false;
	chargeTimer = 4.0f;
	chunk = 0;
}

void Bossbat::init()
{
	if (!isInit)
	{
		worldMat = glm::mat4(1);
		scaleFactor(4, 4, 4);
		isInit = true;
		moveTo(initPos.x, initPos.y, -6.0f);
		invulnTimer = 0.0f;
		movementScale = 0.0f;
		facingRight = true;
		alive = true;
		health = 4;
		slow = false;
		collideRect->update();
		batsToSpawn = 0;
		batTimer = 0.0f;
		chargeTimer = 4.0f;
		charging = false;
		returnPos = chargePos = readPos();
		hasTurned = false;
		rotateTo(0, 3.1415927f / 2.0f, 0);
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

void Bossbat::spawnBat(MapChunk* chunk, float deltaTime)
{
	if (batsToSpawn > 0)
	{
		batTimer -= 1.0*deltaTime;
		if (batTimer < FLT_EPSILON)
		{
			glm::vec3 pos = readPos();
			Bat* newBat = new Bat(glm::vec2(pos.x, pos.y-2.5));
			newBat->setFacing(facingRight);
			newBat->setVisitor();
			chunk->addVisitor(newBat, "Bat");
			delete newBat;
			batsToSpawn--;
			if (batsToSpawn > 0)
				batTimer = 0.3f;
		}
	}
}

int Bossbat::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (chunk = 0)
	{
		int idX, idY;
		map->getChunkIndex(readPos(), &idX, &idY);
		MapChunk** tempHolder = map->getChunks();
		chunk = &tempHolder[idX][idY];
	}
	glm::vec3 pos = readPos();
	if (pos.z < 0.0f)
	{
		moveTo(pos.x, pos.y, pos.z + 10.0*deltaTime);
		if (pos.z >= -1.0f)
		{
			rotateTo(0, -(3.1415927f/2.0f)*deltaTime*10.0f, 0);
		}
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0*deltaTime;
		moveTo(pos.x, pos.y + speed*deltaTime);
		if (collidesWithWorld(chunk))
		{
			moveTo(pos.x, pos.y - speed*deltaTime);
		}
		spawnBat(chunk, deltaTime);
		if (invulnTimer < FLT_EPSILON)
			chargePos = readPos();
	}
	else if (!charging)
	{
		if (pos.y < initPos.y - 0.5f)
		{
			moveTo(pos.x, pos.y + speed*deltaTime);
			if (collidesWithWorld(chunk))
			{
				moveTo(pos.x, pos.y - speed*deltaTime);
			}
		}
		else if (pos.y > initPos.y + 0.5f)
		{
			moveTo(pos.x, pos.y - speed*deltaTime);
			if (collidesWithWorld(chunk))
			{
				moveTo(pos.x, pos.y + speed*deltaTime);
			}
		}
	}
	pos = readPos();
	if (!charging)
	{
		if (facingRight)
		{
			if (movementScale < -1.0f)
				slow = true;
			else if (movementScale > 1.0f)
				slow = true;
			else
				slow = false;

			if (slow)
				moveTo(pos.x + speed*deltaTime * 0.5f, pos.y);
			if (!slow)
				moveTo(pos.x + speed*deltaTime, pos.y);
			movementScale += 1.0*deltaTime;

			if (collidesWithWorld(chunk))
			{
				movementScale -= 1.0*deltaTime;
				if (slow)
				{
					moveTo(pos.x - speed*deltaTime * 0.5, pos.y);
					facingRight = false;
				}
				else
				{
					moveTo(pos.x - speed*deltaTime, pos.y);
					facingRight = false;
				}
			}

			if (movementScale > 1.5f)
				facingRight = false;
			if (!facingRight)
				rotateTo(0, 3.1415927f, 0);
		}
		else
		{
			if (movementScale < -1.0f)
				slow = true;
			else if (movementScale > 1.0f)
				slow = true;
			else
				slow = false;

			if (slow)
				moveTo(pos.x - speed*deltaTime * 0.5f, pos.y);
			if (!slow)
				moveTo(pos.x - speed*deltaTime, pos.y);
			movementScale -= 1.0*deltaTime;

			if (collidesWithWorld(chunk))
			{
				movementScale += 1.0*deltaTime;
				if (slow)
				{
					moveTo(pos.x + speed*deltaTime * 0.5, pos.y);
					facingRight = true;
				}
				else
				{
					moveTo(pos.x + speed*deltaTime, pos.y);
					facingRight = true;
				}
			}

			if (movementScale < -1.5f)
				facingRight = true;
			if (facingRight)
				rotateTo(0, 3.1415927f, 0);
		}
	}
	else
	{
		chargeTimer -= 1.0f*deltaTime;
		float distX = returnPos.x - chargePos.x;
		float distY = returnPos.y - chargePos.y;
		moveTo(pos.x - speed*deltaTime*distX/3, pos.y - speed*deltaTime*distY/3);
		if (collidesWithWorld(chunk) || chargeTimer < FLT_EPSILON)
		{
			moveTo(pos.x + speed*deltaTime*distX/3, pos.y + speed*deltaTime*distY/3);
			charging = false;
			chargeTimer = 4.0f;
		}
	}

	if (!charging)
	{
		chargeTimer -= 1.0f*deltaTime;
		if (chargeTimer < FLT_EPSILON)
		{
			charging = true;
			chargePos = playerPos;
			returnPos = readPos();
			chargeTimer = 0.8f;
			if (chargePos.x < returnPos.x)
			{
				if (facingRight)
				{
					hasTurned = true;
					rotateTo(0, 3.1415927f, 0);
				}
			}
			else if (chargePos.x > returnPos.x)
			{
				if (!facingRight)
				{
					hasTurned = true;
					rotateTo(0, 3.1415927f, 0);
				}
			}
		}
		else if (chargeTimer > 3.5f)
		{
			float distX = chargePos.x - returnPos.x;
			float distY = chargePos.y - returnPos.y;
			moveTo(pos.x - speed*deltaTime*distX / 5, pos.y - speed*deltaTime*distY / 5);
			if (hasTurned)
			{
				hasTurned = false;
				rotateTo(0, 3.1415927f, 0);
			}
			if (collidesWithWorld(chunk))
			{
				moveTo(pos.x + speed*deltaTime*distX / 5, pos.y + speed*deltaTime*distY / 5);
				charging = false;
				chargeTimer = 3.5f;
			}
		}
	}
	return 0;
}

bool Bossbat::isInitiated()
{
	return isInit;
}

void Bossbat::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		batsToSpawn = 3;
		batTimer = 0.6f;
		health -= damage;
		if (health > 0)
		{
			invulnTimer = 1.0f;
			printf("Boss took damage \n");
		}
		else
			alive = false;
	}
}

bool Bossbat::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}

bool Bossbat::isBlinking()
{
	if (invulnTimer > 0.0f)
	{
		int check = int(invulnTimer * 10);
		if (check % 3)
		{
			return true;
		}
	}
	return false;
}