#include "player.h"

void Player::init(Gamepad* pad)
{
	MAX_HP = HP = 3;
	DMG = 1;
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
	currentRune = 0;
	runeEffect = 0;
	shield = 0;
	effectVisible = false;

	// joystick
	if (pad->joyStickDetected()) // joyStick is connected
	{
		joyStickDetected = true;
		gamePad = pad;
	}	
}

Player::~Player()
{
	delete collideRect;
}

void Player::setStartPos(int x, int y)
{
	start = glm::vec2(x, y);
	moveTo(start.x, start.y);
}

void Player::moveWeapon()
{
	vec3 playerPos = readPos();
	int bonusRange = 1;
	if (currentRune == MiscID::rune_range)
		bonusRange = 3;
	if (facingRight)
	{
		weaponMatrix[0].w = playerPos.x + 0.1f + sin(3.14*attackTimer)*bonusRange;
		weaponMatrix[1].w = playerPos.y;
		if (currentRune == MiscID::rune_range || currentRune == MiscID::rune_damage)
		{
			runeEffect->posX = playerPos.x + 0.1f + sin(3.14*attackTimer)*bonusRange;
			runeEffect->posY = playerPos.y;
		}
	}
	else
	{
		weaponMatrix[0].w = playerPos.x - 0.1f - sin(3.14*attackTimer)*bonusRange;
		weaponMatrix[1].w = playerPos.y;
		if (currentRune == MiscID::rune_range || currentRune == MiscID::rune_damage)
		{
			effectVisible;
			runeEffect->posX = playerPos.x - 0.1f - sin(3.14*attackTimer)*bonusRange;
			runeEffect->posY = playerPos.y;
		}
	}
	attackRect.update();
}

int Player::update(UserInput* userInput, Map* map, float deltaTime)
{
	bool isInWeb = false;
	if (map->webbedUp(collideRect, readPos()))
	{
		maxSpeed.x = 3;
		maxSpeed.y = -5;
		isInWeb = true;
	}
	else
	{
		maxSpeed.x = 10;
		maxSpeed.y = -30;
	}
	effectVisible = false;
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
			if ((userInput->getKeyState('A') && !userInput->getKeyState('D'))
				|| gamePad->isButtonPressed(gamePad->Dpad_Left))
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
			if ((userInput->getKeyState('D') && !userInput->getKeyState('A'))
				|| gamePad->isButtonPressed(gamePad->Dpad_Right))
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
			if (((!userInput->getKeyState('A') && !userInput->getKeyState('D')) || (userInput->getKeyState('A') && userInput->getKeyState('D')))
				&& (!gamePad->isButtonPressed(gamePad->Dpad_Left) && !gamePad->isButtonPressed(gamePad->Dpad_Right)))
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
			if (userInput->getKeyState('W') || gamePad->isButtonPressed(gamePad->A) 
				&& noAutoJump)
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
		if (userInput->getKeyState('W') || gamePad->isButtonPressed(gamePad->A)
			&& speed.y > 0 && !isAttacking)
		{
			if (!progressMeter.spiderboss || !isInWeb)
			{
				speed.y -= (acceleration.y * 0.5f);
			}
		}
		else
			speed.y -= acceleration.y;

		if (speed.y < maxSpeed.y)
			speed.y = maxSpeed.y;
		if (speed.y > -maxSpeed.y)
			speed.y = -maxSpeed.y;

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
					if (speed.y < -5.0f)
					{
							float pitch;
							pitch = (maxSpeed.y) / speed.y;
							Audio::getAudio().playSoundPitched(25, pitch, false);
					}
						

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
				moveTo(pos.x - 0.4, pos.y);
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
		if (userInput->getKeyState('A') || gamePad->isButtonPressed(gamePad->Dpad_Left))
		{
			facingRight = false;
			if (speed.x > 0)// && !jumping)
				speed.x = 0;
			speed.x -= acceleration.x;
			if (speed.x < -maxSpeed.x)
				speed.x = -maxSpeed.x;
			moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
		}
		if (userInput->getKeyState('D') || gamePad->isButtonPressed(gamePad->Dpad_Right))
		{
			facingRight = true;
			if (speed.x < 0)// && !jumping)
				speed.x = 0;
			speed.x += acceleration.x;
			if (speed.x > maxSpeed.x)
				speed.x = maxSpeed.x;
			moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
		}
		if (userInput->getKeyState('W') || gamePad->isButtonPressed(gamePad->A))
		{
			if (speed.y < 0)// && !jumping)
				speed.y = 0;
			speed.y += acceleration.x;
			if (speed.y > maxSpeed.x)
				speed.y = maxSpeed.x;
			moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);
		}
		if (userInput->getKeyState('S') || gamePad->isButtonPressed(gamePad->Dpad_Down))
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
			if (currentSpawn)
			{
				currentRune = currentSpawn->getRune();
				if (runeEffect)
				{
					delete runeEffect;
					runeEffect = 0;
				}
				if (currentRune == MiscID::rune_range)
				{
					attackRect.initGameObjectRect(&weaponMatrix, 0.8, 1.5);
					runeEffect = new Light(currentSpawn->lightForPlayer->flameRune);
				}
				else if (currentRune == MiscID::rune_damage)
				{
					DMG += 1;
					runeEffect = new Light(currentSpawn->lightForPlayer->sparkRune);
				}
				else if (currentRune == MiscID::rune_shield)
				{
					shield = 2;
					runeEffect = new Light(currentSpawn->lightForPlayer->forceRune);
				}
			}
			Audio::getAudio().playSound(0, false);//item
		}
	}

	map->giveMeHealthPickup(this, collideRect);

	vec3 playerPos = readPos();
	if (!noclip)
	{
		if (invulnTimer < FLT_EPSILON && !god)
		{
			if (shield == 0 && currentRune == MiscID::rune_shield)
			{
				currentRune = 0;
				delete runeEffect;
				runeEffect = 0;
			}
			glm::vec3 result = map->collideEnemies(collideRect, playerPos);
			if (result.z > -FLT_EPSILON)
			{
				if (currentRune == MiscID::rune_range)
				{
					attackRect.initGameObjectRect(&weaponMatrix, 0.8, 0.9);
					delete runeEffect;
					runeEffect = 0;
					currentRune = 0;
				}
				else if (currentRune == MiscID::rune_damage)
				{
					DMG -= 1;
					delete runeEffect;
					runeEffect = 0;
					currentRune = 0;
				}
				invulnTimer = 1.0f;
				if (HP > 1)
				{
					if (shield == 0)
						HP -= 1;
					printf("Ow, I'm hit! HP remaining is %d\n", HP);
					flinchTimer = 0.3f;
					float recoil = 10.0f;
					if (currentRune == MiscID::rune_shield)
					{
						recoil = 5.0f;
						flinchTimer = 0.1f;
					}
					if (result.x < playerPos.x)
					{
						if (facingRight)
						{
							facingRight = false;
							rotateTo(0, 3.1415927f, 0);
						}
						speed.x = recoil;
						speed.y = recoil;
					}
					else
					{
						if (!facingRight)
						{
							facingRight = true;
							rotateTo(0, 3.1415927f, 0);
						}
						speed.x = -recoil;
						speed.y = recoil;
					}
					animState = "flinch";
					isAttacking = false;
					attackTimer = 0.0f;
					if (currentRune == MiscID::rune_shield)
					{
						shield -= 1;
						Audio::getAudio().playSound(5, false); //player_shield_force
					}
	
				}
				else
				{
					printf("I'm fucking dead!\n");
					map->getBoss(playerPos, true);
					respawn(map);
					Audio::getAudio().playSound(1, false); //player_resurrected
				}
			}
		}
		else
		{
			if (currentRune == MiscID::rune_shield)
			{
				effectVisible = true;
				runeEffect->posX = playerPos.x;
				runeEffect->posY = playerPos.y;
				runeEffect->posZ = playerPos.z;
			}
			invulnTimer -= 1.0f*deltaTime;
			if (flinchTimer > FLT_EPSILON)
				flinchTimer -= 1.0f*deltaTime;
		}
	}
	if (!userInput->getKeyState('W') && !gamePad->isButtonPressed(gamePad->A))
		noAutoJump = true;

	//Attacking
	if (!isAttacking 
		&& (userInput->getSpace() || gamePad->isButtonPressed(gamePad->X)) 
		&& flinchTimer < FLT_EPSILON)
	{
		isAttacking = true;
		attackTimer = 1.0f;
		if (currentRune == MiscID::rune_damage)
			Audio::getAudio().playSound(4, false); //player_attack_ice
		else if (currentRune == MiscID::rune_range)
			Audio::getAudio().playSound(3, false); //player_attack_fire
		else
			Audio::getAudio().playSound(2, false); //player_attack_miss
	}

	if (isAttacking)
	{
		if (currentRune == MiscID::rune_damage || currentRune == MiscID::rune_range)
			effectVisible = true;
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
	map->playerDiedSoRespawnEnemies();
	if (runeEffect)
	{
		delete runeEffect;
		runeEffect = 0;
	}
	HP = MAX_HP;
	shield = 0;
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
		moveTo(start.x, start.y);
	}
}

bool Player::isBlinking() const
{
	if (invulnTimer > 0.0f && !god)
	{
		int check = int(invulnTimer * 10);
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

Light* Player::getRuneLight() const
{
	if (effectVisible)
		return runeEffect;
	return 0;
}

bool Player::isBossDead(std::string boss)
{
	if (boss == "Bossbat")
		return progressMeter.batboss;
	return false;
}

void Player::setProgress(Progress p)
{
	progressMeter.batboss = p.batboss;
	progressMeter.ghostboss = p.ghostboss;
	progressMeter.spiderboss = p.spiderboss;
}

Progress Player::getProgress()
{
	return progressMeter;
}

void Player::getPickup(glm::vec2 chunkIndex)
{
	pickUps++;
	printf("Fick en piece of heart, nu har jag %d\n", pickUps);
	if (pickUps == 3)
	{
		pickUps = 0;
		MAX_HP++;
		Audio::getAudio().playSound(18, false); //item_hearth_completed
		printf("Hittat 3 pickups, nu är nrOfPickups %d och nya MAX_HP är %d\n", pickUps, MAX_HP);
	}
	else
		Audio::getAudio().playSound(17, false); //item_hearth_piece
	HP = MAX_HP;
	printf("HP sattes till %d\n", HP);
	progressMeter.addHealth(chunkIndex);
}

void Player::playerGuiInfo(int* inhp, int* pieces)
{
	*inhp = HP;
	*pieces = pickUps;
}