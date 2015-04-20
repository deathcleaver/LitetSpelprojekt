#include "player.h"


void Player::init()
{
	MAX_HP = HP = 3;
	moveTo(0, 2);
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.7, 1.9);
	speed = vec2(0);
	maxSpeed = vec2(10, -30);
	acceleration = vec2(0.4f,1.3f); // y = gravity
	jumping = false;
	jumpHeight = 6.5f;
	noAutoJump = true;
	landBreak = 0.8f;

	facingRight = true;
	isAttacking = false;
	attackTimer = 0.0f;
	attackRect.initGameObjectRect(&weaponMatrix, 0.6, 0.6);
	weaponMatrix = mat4(1);
}

Player::~Player()
{
	delete collideRect;
}

void Player::moveWeapon()
{
	vec3 playerPos = readPos();
	if (facingRight)
	{
		weaponMatrix[0].w = playerPos.x + sin(3.14*attackTimer);
		weaponMatrix[1].w = playerPos.y;
	}
	else
	{
		weaponMatrix[0].w = playerPos.x - sin(3.14*attackTimer);
		weaponMatrix[1].w = playerPos.y;
	}
	attackRect.update();
}

int Player::update(UserInput* userInput, Map* map, float deltaTime)
{
	timepass += deltaTime;

	// update pos & camera using user input
	vec3 lastPos = readPos();
	vec3 tempPos = lastPos;
	bool result = false;

	//Toggle God
	if (userInput->getKeyState('G'))
	{
		if (!god)
		{
			god = true;
			printf("I AM BECOME GOD\n");
		}
	}
	if (userInput->getKeyState('M'))
	{
		if (god)
		{
			god = false;
			printf("I GAVE UP IMMORTALITY FOR THIS?!\n");
		}
	}

	//Toggle noclip
	if (userInput->getKeyState('C'))
	{
		if (noclip)
		{
			noclip = false;
			printf("Return to the physical realm\n");
			speed.x = speed.y = 0;
		}
	}
	if (userInput->getKeyState('N'))
	{
		if (!noclip)
		{
			noclip = true;
			printf("Ascend this world, child\n");
		}
	}
	if (!noclip)
	{
		//MoveX
		//left
		if (!isAttacking)
		{
			if (userInput->getKeyState('A') && !userInput->getKeyState('D'))
			{
				facingRight = false;
				if (flinchTimer < FLT_EPSILON)
				{
					if (speed.x > 0)// && !jumping)
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
				facingRight = true;
				if (flinchTimer < FLT_EPSILON)
				{
					if (speed.x < 0)// && !jumping)
					{
						speed.x = 0;
					}
					speed.x += acceleration.x;
				}
				if (speed.x > maxSpeed.x)
					speed.x = maxSpeed.x;

				moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
			}

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
		if (!isAttacking)
		{
			if (userInput->getKeyState('W') && noAutoJump)
			{
				if (!jumping && flinchTimer < FLT_EPSILON)
				{
					noAutoJump = false;
					speed.y = jumpHeight * 3;
					jumping = true;
				}
			}
		}


		//gravity
		if (userInput->getKeyState('W') && speed.y > 0 && !isAttacking)
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
					speed.x *= landBreak;
					jumping = false;
					speed.y = 0;
				}
			}
			else
			{
				speed.y = 0;
			}
			moveTo(tempPos.x, lastPos.y);
			collideRect->update();
		}
		else
		{
			jumping = true;
		}
	}
	else
	{
		if (userInput->getKeyState('A'))
		{
			facingRight = false;
			if (speed.x > 0)// && !jumping)
				speed.x = 0;
			speed.x -= acceleration.x;
			if (speed.x < -maxSpeed.x)
				speed.x = -maxSpeed.x;
			moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
		}
		if (userInput->getKeyState('D'))
		{
			facingRight = true;
			if (speed.x < 0)// && !jumping)
				speed.x = 0;
			speed.x += acceleration.x;
			if (speed.x > maxSpeed.x)
				speed.x = maxSpeed.x;
			moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
		}
		if (userInput->getKeyState('W'))
		{
			if (speed.y < 0)// && !jumping)
				speed.y = 0;
			speed.y += acceleration.x;
			if (speed.y > maxSpeed.x)
				speed.y = maxSpeed.x;
			moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);
		}
		if (userInput->getKeyState('S'))
		{
			if (speed.y > 0)// && !jumping)
				speed.y = 0;
			speed.y -= acceleration.x;
			if (speed.y < -maxSpeed.x)
				speed.y = -maxSpeed.x;
			moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);
		}
		collideRect->update();
	}

	if (map->collideShrine(collideRect, readPos(), currentSpawn))
	{
		if (HP != MAX_HP)
		{
			HP = MAX_HP;
			printf("Max HP regained\n");
		}
	}

	vec3 playerPos = readPos();
	if (!noclip)
	{
		if (invulnTimer < FLT_EPSILON && !god)
		{
			glm::vec3 result = map->collideEnemies(collideRect, playerPos);
			if (result.z > -FLT_EPSILON)
			{
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
					isAttacking = false;
					attackTimer = 0.0f;
				}
				else
				{
					printf("I'm fucking dead!\n");
					respawn(map);
				}
			}
		}
		else
		{
			invulnTimer -= 1.0f*deltaTime;
			if (flinchTimer > FLT_EPSILON)
				flinchTimer -= 1.0f*deltaTime;
		}
	}
	if (!userInput->getKeyState('W'))
		noAutoJump = true;

	//Attacking
	if (!isAttacking && userInput->getSpace())
	{
		isAttacking = true;
		attackTimer = 1.0f;
		speed.x = 0.0f;
	}
	
	if (isAttacking)
	{
		moveWeapon();
		map->attackEnemies(&attackRect, playerPos);
		attackTimer -= 2.0*deltaTime;
		if (attackTimer < FLT_EPSILON)
		{
			isAttacking = false;
		}
	}
	return 0;
}

vec2 Player::getSpeed()
{
	return speed;
}

void Player::respawn(Map* map)
{
	HP = 3;
	speed = vec2(0);
	jumping = false;
	if (currentSpawn != 0)
	{
		moveTo(currentSpawn->getPos().x, currentSpawn->getPos().y);
		printf("Jag hade en respawnpunkt\n");
	}
	else
	{
		moveTo(0, 2);
	}
	map->playerDiedSoRespawnEnemies(readPos());
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
