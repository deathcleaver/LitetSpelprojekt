#include "player.h"


void Player::init()
{
	HP = 3;
	moveTo(0, 2);
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9, 1.9);
	speed = vec2(0);
	maxSpeed = vec2(15, -30);
	acceleration = vec2(0.4f,0.8f); // y = gravity
	jumping = false;
	jumpHeight = 5.2f;
	noAutoJump = true;
}

Player::~Player()
{
	delete collideRect;
}

int Player::update(UserInput* userInput, Map* map, float deltaTime)
{
	timepass += deltaTime;

	// update pos & camera using user input
	vec3 lastPos = readPos();
	vec3 tempPos = lastPos;
	bool result = false;

	//Toggle God

	//MoveX
	//left
	if (userInput->getKeyState('A') && !userInput->getKeyState('D'))
	{
		if (flinchTimer < FLT_EPSILON)
		{
			if (speed.x > 0 && !jumping)
			{
				speed.x = 0;
			}
			speed.x -= acceleration.x;
		}
		if (speed.x < -maxSpeed.x)
			speed.x = -maxSpeed.x;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
	}

	//right
	if (userInput->getKeyState('D') && !userInput->getKeyState('A'))
	{
		if (flinchTimer < FLT_EPSILON)
		{
			if (speed.x < 0 && !jumping)
			{
				speed.x = 0;
			}
			speed.x += acceleration.x;
		}
		if (speed.x > maxSpeed.x)
			speed.x = maxSpeed.x;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);}

	//stop
	if (!userInput->getKeyState('A') && !userInput->getKeyState('D') ||
		userInput->getKeyState('A') && userInput->getKeyState('D'))
	{
		if (flinchTimer < FLT_EPSILON)
		{
			if (!jumping)
			{
				speed.x = 0;
			}
		}
		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
	}

	//update collide rect
	if (readPos() != lastPos)
	{
		collideRect->update();
		result = map->collideMap(collideRect, readPos());


		if (result) //collide, move back X
		{
			tempPos.x = lastPos.x;
			moveTo(lastPos.x, tempPos.y);
			speed.x = 0;
			result = false;
		}
	}

	//MoveY
	if (userInput->getKeyState('W') && noAutoJump)
	{
		if (!jumping && flinchTimer < FLT_EPSILON)
		{
			noAutoJump = false;
			speed.y = jumpHeight * 3;
			jumping = true;
		}
	}

	
	//gravity
	if (userInput->getKeyState('W') && speed.y > 0)
		speed.y -= (acceleration.y * 0.5f);
	else
		speed.y -= acceleration.y;
	
	if (speed.y < maxSpeed.y)
		speed.y = maxSpeed.y;

	moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);

	//update collide rect
	collideRect->update();
	vec3 pos = readPos();
	result = map->collideMap(collideRect, pos);

	if (result) //collide, move back Y
	{
		if (jumping)
		{
			if (lastPos.y < pos.y)
			{
				speed.y = 0;
			}
			else
			{
				jumping = false;
				speed.y = 0;
			}
		}
		else
		{
			speed.y = 0;
		}
		moveTo(tempPos.x, lastPos.y);
		result = false;
	}
	else
	{
		jumping = true;
	}

	map->collideShrine(collideRect, readPos(), currentSpawn);

	if (invulnTimer < FLT_EPSILON && !god)
	{
		vec3 playerPos = readPos();
		glm::vec3 result = map->collideEnemies(collideRect, playerPos);
		if (result.z > -FLT_EPSILON)
		{
			printf("Enemy x/y/z: %f/%f/%f \n", result.x, result.y, result.z);
			printf("Player x/y/z: %f/%f/%f \n", playerPos.x, playerPos.y, playerPos.z);
			invulnTimer = 1.0f;
			if (HP > 1)
			{
				HP -= 1;
				printf("Ow, I'm hit! HP remaining is %d\n", HP);
				flinchTimer = 0.3f;
				if (result.x < playerPos.x)
				{
					speed.x = 10;
					speed.y = 10;
				}
				else
				{
					speed.x = -10;
					speed.y = 10;
				}
			}
			else
			{
				printf("I'm fucking dead!\n");
				respawn();
			}
		}
	}
	else
	{
		invulnTimer -= 1.0f*deltaTime;
		if (flinchTimer > FLT_EPSILON)
			flinchTimer -= 1.0f*deltaTime;
	}
	//map->getChunkIndex(vec2(readPos().x, readPos().y), &idX, &idY);
	//if (idX != -1 && idY != -1)
	//{
	//	if (map->getChunks()[idX][idY].collide(collideRect)) //collideRect->intersects()
	//	{
	//		result = true;
	//		printf("collision in chunk: %i.%i. Pos %f,%f\n", idX, idY, readPos().x, readPos().y);
	//	}
	//	else
	//		printf("no collision%f,%f\n", readPos().x, readPos().y);
	//}
	//else
	//	printf("out of map%f,%f\n", readPos().x, readPos().y);
	if (userInput->getKeyState('W') == false)
		noAutoJump = true;
	return 0;
}

vec2 Player::getSpeed()
{
	return speed;
}

void Player::respawn()
{
	HP = 3;
	speed = vec2(0);
	jumping = false;
	if (currentSpawn != 0)
	{
		moveTo(currentSpawn->getPos().x, currentSpawn->getPos().y);
		printf("Jag hade en respawnpunkt");
	}
	else
	{
		moveTo(0, 2);
	}
}

bool Player::isBlinking() const
{
	if (invulnTimer > 0.0f && !god)
	{
		int check = int(invulnTimer*10);
		if (check % 3)
		{
			return true;
		}
	}
	return false;
}