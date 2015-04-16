#include "player.h"

#include <stdio.h>


void Player::init()
{
	HP = 3;
	moveTo(0, 2);
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 2);

	speed = vec2(0);
	maxSpeed = vec2(15, 30);
	acceleration = vec2(0.05f,0.5f); // y = gravity
	jumping = false;
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
	int idX = 0, idY = 0;
	bool result = false;

	//MoveX
	//right
	if (userInput->getKeyState('A') && !userInput->getKeyState('D')) {
		speed.x -= acceleration.x;
		if (speed.x < -maxSpeed.x)
			speed.x = -maxSpeed.x;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);}

	//left
	if (userInput->getKeyState('D') && !userInput->getKeyState('A')){
		speed.x += acceleration.x;
		if (speed.x > maxSpeed.x)
			speed.x = maxSpeed.x;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);}

	//stop
	if (!userInput->getKeyState('A') && !userInput->getKeyState('D') ||
		userInput->getKeyState('A') && userInput->getKeyState('D'))
	{
		if (speed.x + 0.5 < 0)
			speed.x += acceleration.x * 10;
		
		else if (speed.x - 0.5 > 0)
			speed.x -= acceleration.x * 10;
		else
			speed.x = 0;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
	}

	//update collide rect
	collideRect->update();

	map->getChunkIndex(readPos(), &idX, &idY);
	if (idX != -1 && idY != -1)
		result = map->getChunks()[idX][idY].collide(collideRect);

	if (result) //collide, move back X
	{
		tempPos.x = lastPos.x;
		moveTo(lastPos.x, tempPos.y);
		result = false;
	}

	//MoveY
	if (userInput->getKeyState('W'))
	{
		if (jumping == false && speed.y <= 2)
		{
			speed.y = 75;
			moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);
			jumping = true;
		}
	}
		
	if (userInput->getKeyState('S'))
		moveTo(tempPos.x, tempPos.y -= speed.y * deltaTime, 0);

	//gravity
	if (speed.y > 0)
		speed.y -= acceleration.y * 0.2;
	else
		speed.y -= acceleration.y * 0.2;
	
	if (speed.y > maxSpeed.y)
		speed.y = maxSpeed.y;

	moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);

	//update collide rect
	collideRect->update();

	map->getChunkIndex(readPos(), &idX, &idY);
	if (idX != -1 && idY != -1)
		result = map->getChunks()[idX][idY].collide(collideRect);

	if (result) //collide, move back Y
	{
		if (jumping = true)
		{
			jumping = false;
			speed.y = 0;
		}
		tempPos.y = lastPos.y;
		moveTo(tempPos.x, lastPos.y);
		result = false;
	}

<<<<<<< HEAD
	map->getChunkIndex(readPos(), &idX, &idY);
	if (idX != -1 && idY != -1)
		result = map->getChunks()[idX][idY].playerVsEnemies(collideRect);
	if (result)
	{
		
		if (HP > 1)
		{
			HP -= 1;
			printf("Ow, I'm hit! HP remaining is %d\n", HP);
		}
		else
		{
			printf("I'm fucking dead!\n");
		}
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

=======
>>>>>>> Camera offset now works
	printf("Speed: %fx,%fy\n", speed.x, speed.y);

	return 0;
}

vec2 Player::getSpeed()
{
	return speed;
}