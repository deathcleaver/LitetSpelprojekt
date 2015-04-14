#include "player.h"



void Player::init()
{
	scaleUniformAD(-0.98f);
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
	}
	else if (userInput->getKeyState('D'))
	{
		moveTo(pos.x += 0.005, 0, 0);
	}
	return 0;
}
