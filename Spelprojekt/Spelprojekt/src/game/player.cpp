#include "player.h"



void Player::init()
{
	
}

int Player::update(float deltaTime)
{
	timepass += deltaTime;
	rotate(0, deltaTime, 0);
	moveTo(sin(timepass) * 13, sin(timepass*2) * 5, 0);
	return 0;
}
