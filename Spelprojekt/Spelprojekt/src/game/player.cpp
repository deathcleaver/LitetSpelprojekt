#include "player.h"


void Player::init()
{
	MAX_HP = HP = 3;
	DMG = 1;
	moveTo(0, 2);
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.7, 1.9);
	speed = vec2(0);
	maxSpeed = vec2(10, -30);
	acceleration = vec2(0.4f, 1.35f); // y = gravity
	jumping = false;
	doubleJump = false;
	jumpHeight = 6.0f;
	noAutoJump = true;
	landBreak = 0.8f;
	facingRight = true;
	isAttacking = false;
	attackTimer = 0.0f;
	attackRect.initGameObjectRect(&weaponMatrix, 0.8, 0.9);
	weaponMatrix = mat4(1);
	animState = "idle";
	bossFighting = false;
	currentRune = NORUNE;
}

Player::~Player()
{
	delete collideRect;
}

void Player::moveWeapon()
{
	vec3 playerPos = readPos();
	int bonusRange = 1;
	if (currentRune == FLAME)
		bonusRange = 5;
	if (facingRight)
	{
		weaponMatrix[0].w = playerPos.x +0.1f + sin(3.14*attackTimer)*bonusRange;
		weaponMatrix[1].w = playerPos.y;
	}
	else
	{
		weaponMatrix[0].w = playerPos.x -0.1f - sin(3.14*attackTimer)*bonusRange;
		weaponMatrix[1].w = playerPos.y;
	}
	attackRect.update();
}

int Player::update(UserInput* userInput, Map* map, float deltaTime)
{
	animState = "idle";
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
				if (facingRight)
				{
					facingRight = false;
					rotateTo(0, 3.1415927f, 0);
				}
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
				if (!jumping)
					animState = "walk";
				else
					animState = "air";
			}
			//right
			if (userInput->getKeyState('D') && !userInput->getKeyState('A'))
			{
				if (!facingRight)
				{
					facingRight = true;
					rotateTo(0, 3.1415927f, 0);
				}
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
				if (!jumping)
					animState = "walk";
				else
					animState = "air";
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
		else  //is attacking, but movement continues in air
		{
			if (jumping)
			{
				moveTo(tempPos.x += speed.x*deltaTime, tempPos.y, 0);
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
				if (jumping && !doubleJump && progressMeter.batboss && flinchTimer < FLT_EPSILON)
				{
					noAutoJump = false;
					speed.y = jumpHeight * 2.5;
					doubleJump = true;
				}
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
					doubleJump = false;
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
		if (bossFighting) //YOU CAN'T RUN FROM A TRAINER BATTLE
		{
			pos = readPos();
			vec3 chunkMid = map->getChunkMiddle(lastPos);
			if (pos.x < chunkMid.x - 17.5f)
			{
				moveTo(pos.x + 0.4, pos.y);
				speed.x = -speed.x;
			}
			else if (pos.x > chunkMid.x + 17.5f)
			{
				moveTo(pos.x -0.4, pos.y);
				speed.x = -speed.x;
			}
			if (pos.y < chunkMid.y - 17.5f)
			{
				moveTo(pos.x, pos.y + 0.4);
				speed.y = -speed.y;
			}
			else if (pos.y > chunkMid.y + 17.5f)
			{
				moveTo(pos.x, pos.y - 0.4);
				speed.y = -speed.y;
			}
		}
	}
	else //noclip is on
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
		if (currentRune == 0)
		{
			currentRune = currentSpawn->getRune();
			if (currentRune == FLAME)
				attackRect.initGameObjectRect(&weaponMatrix, 0.8, 2.5);
			else if (currentRune == SPARK)
				DMG += 1;
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
				if (currentRune == FLAME)
					attackRect.initGameObjectRect(&weaponMatrix, 0.8, 0.9);
				else if (currentRune == SPARK)
					DMG -= 1;
				currentRune = NORUNE;
				invulnTimer = 1.0f;
				if (HP > 1)
				{
					HP -= 1;
					printf("Ow, I'm hit! HP remaining is %d\n", HP);
					flinchTimer = 0.3f;
					if (result.x < playerPos.x)
					{
						if (facingRight)
						{
							facingRight = false;
							rotateTo(0, 3.1415927f, 0);
						}
						speed.x = 10;
						speed.y = 10;
					}
					else
					{
						if (!facingRight)
						{
							facingRight = true;
							rotateTo(0, 3.1415927f, 0);
						}
						speed.x = -10;
						speed.y = 10;
					}
					animState = "flinch";
					isAttacking = false;
					attackTimer = 0.0f;
				}
				else
				{
					printf("I'm fucking dead!\n");
					map->getBoss(playerPos, true);
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
	if (!isAttacking && userInput->getSpace() && flinchTimer < FLT_EPSILON)
	{
		isAttacking = true;
		attackTimer = 1.0f;
	}
	
	if (isAttacking)
	{
		animState = "attack";
		moveWeapon();
		map->attackEnemies(&attackRect, playerPos, DMG);
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

bool Player::isBossFighting()
{
	return bossFighting;
}

void Player::respawn(Map* map)
{
	HP = 3;
	speed = vec2(0);
	jumping = false;
	doubleJump = false;
	if (currentSpawn != 0)
	{
		moveTo(currentSpawn->getPos().x, currentSpawn->getPos().y);
		printf("Jag hade en respawnpunkt\n");
	}
	else
	{
		moveTo(0, 2);
	}
	map->playerDiedSoRespawnEnemies();
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

std::string Player::getAnimState()
{
	return animState;
}

void Player::execute(Map* map)
{
	respawn(map);
}

void Player::fightThatBossBro()
{
	bossFighting = true;
}

void Player::dingDongTheBossIsDead(std::string boss)
{
	bossFighting = false;
	progressMeter.checkBossType(boss);
}