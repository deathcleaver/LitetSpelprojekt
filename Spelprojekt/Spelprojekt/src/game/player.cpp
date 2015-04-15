#include "player.h"



void Player::init()
{
	
}

int Player::update(float deltaTime)
{
	timepass += deltaTime;
	rotate(0, deltaTime, 0);
	moveTo(sin(timepass) * 13, sin(timepass * 2) * 5, 0);

	return 0;
}

int Player::update(UserInput* userInput, float deltaTime)
{
	timepass += deltaTime;

	vec3 pos = readPos();

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

	return 0;
}

int Player::getDir()
{
	return dir;
}