#include "player.h"

#include <stdio.h>


void Player::init()
{
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 2);
}

int Player::update(float deltaTime)
{
	timepass += deltaTime;
	rotate(0, deltaTime, 0);
	moveTo(sin(timepass) * 13, sin(timepass * 2) * 5, 0);

	//update collide rect
	collideRect->update();

	return 0;
}

int Player::update(UserInput* userInput, Map* map, float deltaTime)
{
	timepass += deltaTime;

	// update pos & camera using user input
	vec3 tempPos = readPos();

	//WASD
	if (userInput->getKeyState('A'))
	{
		moveTo(tempPos.x -= 0.110, 0, 0);
		dir = -1;
	}
	else if (userInput->getKeyState('D'))
	{
		moveTo(tempPos.x += 0.110, 0, 0);
		dir = 1;
	}
	else
		dir = 0;

	//update collide rect
	collideRect->update();

	int idX = 0, idY = 0;

	map->getChunkIndex(vec2(readPos().x, readPos().y), &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		
		if (map->getChunks()[idX][idY].collide(collideRect)) //collideRect->intersects()
		{
			printf("collision in chunk: %i.%i. Pos %f,%f\n", idX, idY, readPos().x, readPos().y);
		}
		else
			printf("no collision%f,%f\n", readPos().x, readPos().y);
	}
	else
	{
		printf("out of map%f,%f\n", readPos().x, readPos().y);
	}
	return 0;
}

int Player::getDir()
{
	return dir;
}