#include "player.h"



void Player::init()
{
	
}

int Player::update(float deltaTime)
{
	timepass += deltaTime;
	rotate(0, deltaTime, 0);
	moveTo(1 * 13, 1 * 5, 0);
	return 0;
}
