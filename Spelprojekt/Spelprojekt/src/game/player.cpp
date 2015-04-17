#include "player.h"


void Player::init()
{
	HP = 3;
	moveTo(0, 2);
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9, 1.9);
	speed = vec2(0);
	maxSpeed = vec2(15, -30);
	acceleration = vec2(0.2f,0.4f); // y = gravity
	jumping = false;
	jumpHeight = 25.0f;
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

	//MoveX
	//left
	if (userInput->getKeyState('A') && !userInput->getKeyState('D')) {
		if (speed.x > 0 && !jumping)
		{
			speed.x = 0;
		}
		speed.x -= acceleration.x;
		if (speed.x < -maxSpeed.x)
			speed.x = -maxSpeed.x;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
	}

	//right
	if (userInput->getKeyState('D') && !userInput->getKeyState('A')){
		if (speed.x < 0 && !jumping)
		{
			speed.x = 0;
		}
		speed.x += acceleration.x;
		if (speed.x > maxSpeed.x)
			speed.x = maxSpeed.x;

		moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);}

	//stop
	if (!userInput->getKeyState('A') && !userInput->getKeyState('D') ||
		userInput->getKeyState('A') && userInput->getKeyState('D'))
	{
		if (!jumping)
		{
			speed.x = 0;
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
			result = false;
		}
	}

	//MoveY
	if (userInput->getKeyState('W'))
	{
		if (!jumping)
		{
			speed.y = jumpHeight;
			jumping = true;
		}
	}

	//gravity
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
		moveTo(tempPos.x, lastPos.y);
		result = false;
	}
	else
	{
		jumping = true;
	}

	result = map->collideEnemies(collideRect, readPos());
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
	return 0;
}

vec2 Player::getSpeed()
{
	return speed;
}