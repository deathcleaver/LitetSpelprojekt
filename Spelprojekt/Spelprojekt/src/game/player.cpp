#include "player.h"

#include <stdio.h>


void Player::init()
{
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 2);
	
	testMat = worldMat;
	
	testRect = new Rect();
	//testRect->initGameObjectRect(&testMat, 100, 100);
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
	vec3 pos = readPos();


	//WASD
	if (userInput->getKeyState('A'))
	{
		moveTo(pos.x -= 0.005, 0, 0);
		dir = -1;
	}
	else if (userInput->getKeyState('D'))
	{
		moveTo(pos.x += 0.005, 0, 0);
		dir = 1;
	}
	else
		dir = 0;

	//update collide rect
	collideRect->update();
	testRect->update();

	if (collideRect->intersects(testRect))
	{
		printf("collision\n");
	}
	else
		printf("no collision\n");

	return 0;
}

int Player::getDir()
{
	return dir;
}