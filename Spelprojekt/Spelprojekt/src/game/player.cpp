#include "player.h"
#include "../GUI.h"

void Player::init()
{
	MAX_HP = HP = 3;
	DMG = 1;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.6f, 1.8f);
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
	attackRect.initGameObjectRect(&weaponMatrix, 0.8f, 0.9f);
	weaponMatrix = mat4(1);
	animState = "idle";
	bossFighting = false;
	currentRune = 0;
	shield = 0;
	effectVisible = false;

	runeEffect = new Effect();

	teleportTime = -1.0f;

}

Player::~Player()
{
	delete runeEffect;
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
		weaponMatrix[0].w = playerPos.x + 0.1f + sin(3.14f*attackTimer)*bonusRange;
		weaponMatrix[1].w = playerPos.y;
		if (currentRune == MiscID::rune_range || currentRune == MiscID::rune_damage)
		{
			runeEffect->getEffect()->setSpawn(playerPos.x + 0.1f + sin(3.14f*attackTimer)*bonusRange, playerPos.y, 0);
			//runeEffect->posX = playerPos.x + 0.1f + sin(3.14*attackTimer)*bonusRange;
			//runeEffect->posY = playerPos.y;
		}
	}
	else
	{
		weaponMatrix[0].w = playerPos.x - 0.1f - sin(3.14f*attackTimer)*bonusRange;
		weaponMatrix[1].w = playerPos.y;
		if (currentRune == MiscID::rune_range || currentRune == MiscID::rune_damage)
		{
			effectVisible;
			//runeEffect->posX = playerPos.x - 0.1f - sin(3.14*attackTimer)*bonusRange;
			runeEffect->getEffect()->setSpawn(playerPos.x - 0.1f - sin(3.14f*attackTimer)*bonusRange, playerPos.y, 0);
			//runeEffect->posY = playerPos.y;
		}
	}
	attackRect.update();
}

int Player::update(UserInput* userInput, Gamepad* pad, Map* map, GUI* gui, float deltaTime)
{
	if (panningToMirror == 1)
	{
		flinchTimer -= 1.0*deltaTime;
		if (flinchTimer < FLT_EPSILON)
			panningToMirror = -1;
		return 1;
	}
	else if (panningToMirror == 0 && flinchTimer > FLT_EPSILON)
	{
		flinchTimer -= 1.0*deltaTime;
		if (flinchTimer < FLT_EPSILON)
		{
			flinchTimer = 4.0f;
			panningToMirror = 1;
		}
	}
	// pad axes values
	float axesX, axesY;
	if (pad->joyStickDetected())
	{
		pad->getAxesValues(0, axesX, axesY); // left stick
	}
	else
	{
		axesX = 0;
		axesY = 0;
	}

	bool isInWeb = false;
	if (map->webbedUp(collideRect, readPos()))
	{
		maxSpeed.x = 6;
		maxSpeed.y = -5;
		isInWeb = true;
	}
	else
	{
		maxSpeed.x = 10;
		maxSpeed.y = -30;
	}

	if (isAttacking && !jumping)
	{
		maxSpeed.x = 4;
	}
	else
	{
		maxSpeed.x = 10;
	}
	effectVisible = false;
	if (!jumping)
		animState = "idle";
	else
		animState = "air";
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
			Debug::DebugOutput("I AM BECOME GOD\n");
		}
	}
	if (userInput->getKeyState('M'))
	{
		if (god)
		{
			god = false;
			Debug::DebugOutput("I GAVE UP IMMORTALITY FOR THIS?!\n");
		}
	}

	//Toggle noclip
	if (userInput->getKeyState('C') || pad->isButtonPressed(pad->getButtons().L3))
	{
		if (noclip)
		{
			noclip = false;
			Debug::DebugOutput("Return to the physical realm\n");
			speed.x = speed.y = 0;
		}
	}
	if (userInput->getKeyState('N') || pad->isButtonPressed(pad->getButtons().R3))
	{
		if (!noclip)
		{
			noclip = true;
			Debug::DebugOutput("Ascend this world, child\n");
		}
	}
	if (!noclip)
	{
		//MoveX
		//left
		if ((userInput->getKeyState('A') && !userInput->getKeyState('D'))
			|| pad->isButtonPressed(pad->getButtons().Dpad_Left)
			|| axesX < -pad->getDeadzone())
		{
			if (facingRight && !isAttacking)
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

			if (axesX < -pad->getDeadzone()) // pad stick movement
				moveTo(tempPos.x += (speed.x * -axesX) * deltaTime, tempPos.y, 0);
			else
				moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);

			if (!jumping)
				animState = "walk";
			else
				animState = "air";
		}
		//right
		if ((userInput->getKeyState('D') && !userInput->getKeyState('A'))
			|| pad->isButtonPressed(pad->getButtons().Dpad_Right)
			|| axesX > pad->getDeadzone())
		{
			if (!facingRight && !isAttacking)
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
				
			if (axesX > pad->getDeadzone()) // pad stick movement
				moveTo(tempPos.x += (speed.x * axesX) * deltaTime, tempPos.y, 0);
			else
				moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);

			if (!jumping)
				animState = "walk";
			else
				animState = "air";
		}

		//stop
		if (((!userInput->getKeyState('A') && !userInput->getKeyState('D')) || (userInput->getKeyState('A') && userInput->getKeyState('D')))
			&& (!pad->isButtonPressed(pad->getButtons().Dpad_Left) && !pad->isButtonPressed(pad->getButtons().Dpad_Right))
			&& axesX == 0)
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
			if ((userInput->getKeyState('W') || pad->jump(axesY))
				&& noAutoJump)
			{
				if (jumping && !doubleJump && progressMeter.batboss && flinchTimer < FLT_EPSILON)
				{
					noAutoJump = false;
					speed.y = jumpHeight * 2.5f;
					doubleJump = true;
				}
				if (!jumping && flinchTimer < FLT_EPSILON)
				{
					noAutoJump = false;
					speed.y = jumpHeight * 3.0f;
					jumping = true;
				}
			}
		}


		//gravity
		if (userInput->getKeyState('W') || pad->climb(axesY))
		{
			if (progressMeter.spiderboss && isInWeb)
				speed.y += acceleration.y*1.2f;
			else if (speed.y > 0.0)
				speed.y += acceleration.y * 0.5f;
		}
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
							Audio::getAudio().playSound(SoundID::player_landing, false, pitch);
					}

					speed.x *= landBreak;
					doubleJump = false;
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
		if (bossFighting) //YOU CAN'T RUN FROM A TRAINER BATTLE
		{
			pos = readPos();
			vec3 chunkMid = map->getChunkMiddle(lastPos);
			if (pos.x < chunkMid.x - 17.5f)
			{
				moveTo(pos.x + 0.4f, pos.y);
				speed.x = -speed.x;
			}
			else if (pos.x > chunkMid.x + 17.5f)
			{
				moveTo(pos.x - 0.4f, pos.y);
				speed.x = -speed.x;
			}
			if (pos.y < chunkMid.y - 17.5f)
			{
				moveTo(pos.x, pos.y + 0.4f);
				speed.y = -speed.y;
			}
			else if (pos.y > chunkMid.y + 17.5f)
			{
				moveTo(pos.x, pos.y - 0.4f);
				speed.y = -speed.y;
			}
		}
	}
	else //noclip is on
	{
		if (userInput->getKeyState('A') || pad->isButtonPressed(pad->getButtons().Dpad_Left))
		{
			facingRight = false;
			if (speed.x > 0)// && !jumping)
				speed.x = 0;
			speed.x -= acceleration.x;
			if (speed.x < -maxSpeed.x*3)
				speed.x = -maxSpeed.x;
			moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
		}
		if (userInput->getKeyState('D') || pad->isButtonPressed(pad->getButtons().Dpad_Right))
		{
			facingRight = true;
			if (speed.x < 0)// && !jumping)
				speed.x = 0;
			speed.x += acceleration.x;
			if (speed.x > maxSpeed.x*3)
				speed.x = maxSpeed.x;
			moveTo(tempPos.x += speed.x * deltaTime, tempPos.y, 0);
		}
		if (userInput->getKeyState('W') || pad->isButtonPressed(pad->getButtons().Dpad_Up))
		{
			if (speed.y < 0)// && !jumping)
				speed.y = 0;
			speed.y += acceleration.x;
			if (speed.y > maxSpeed.x*3)
				speed.y = maxSpeed.x;
			moveTo(tempPos.x, tempPos.y += speed.y * deltaTime, 0);
		}
		if (userInput->getKeyState('S') || pad->isButtonPressed(pad->getButtons().Dpad_Down))
		{
			if (speed.y > 0)// && !jumping)
				speed.y = 0;
			speed.y -= acceleration.x;
			if (speed.y < -maxSpeed.x*3)
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
			Debug::DebugOutput("Max HP regained\n");
			Audio::getAudio().playSound(SoundID::player_healed, false);// player_healed
		}

		if (currentRune == 0)
		{
			if (currentSpawn)
			{
				vec3 playerPos = readPos();
				currentRune = currentSpawn->getRune();
				if (runeEffect)
				{
					//delete runeEffect;
					//runeEffect = 0;
				}
				if (currentRune == MiscID::rune_range)
				{
					attackRect.initGameObjectRect(&weaponMatrix, 0.8f, 1.5f);
					runeEffect->reCreate(EffectType::torch);
					runeEffect->getEffect()->init(playerPos.x, playerPos.y, playerPos.z);
					Audio::getAudio().playSound(SoundID::rune_recieved, false);// rune_recieved
					//runeEffect = new Light(currentSpawn->lightForPlayer->flameRune);
				}
				else if (currentRune == MiscID::rune_damage)
				{
					DMG += 1;
					runeEffect->reCreate(EffectType::spark);
					runeEffect->getEffect()->init(playerPos.x, playerPos.y, playerPos.z);
					Audio::getAudio().playSound(SoundID::rune_recieved, false);// rune_recieved
					//runeEffect = new Light(currentSpawn->lightForPlayer->sparkRune);
				}
				else if (currentRune == MiscID::rune_shield)
				{
					shield = 2;
					runeEffect->reCreate(EffectType::shield);
					runeEffect->getEffect()->init(playerPos.x, playerPos.y, playerPos.z);
					Audio::getAudio().playSound(SoundID::rune_recieved, false);// rune_recieved
					//runeEffect = new Light(currentSpawn->lightForPlayer->forceRune);
				}
			}
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
				//delete runeEffect;
				//runeEffect = 0;
			}
			glm::vec3 result = map->collideEnemies(collideRect, playerPos);
			if (result.z > -FLT_EPSILON)
			{
				if (currentRune == MiscID::rune_range)
				{
					attackRect.initGameObjectRect(&weaponMatrix, 0.8f, 0.9f);
					//delete runeEffect;
					//runeEffect = 0;
					currentRune = 0;
				}
				else if (currentRune == MiscID::rune_damage)
				{
					DMG -= 1;
					
					//delete runeEffect;
					//runeEffect = 0;
					currentRune = 0;
				}
				invulnTimer = 1.0f;
				if (HP > 1)
				{
					if (shield == 0)
					{
						switch (GameConfig::get().configDifficulty)
						{
						case GameConfig::Casual:
							HP -= 1;
							break;
						case GameConfig::Hardcore:
							HP -= 2;
							break;
						case GameConfig::DmonInHell:
							HP -= 2;
							break;
						}

						// check required if diff is higher than Casual
						if (HP <= 0)
						{
							gui->showNewUpgrade(0);
							Debug::DebugOutput("I'm fucking dead!\n");
							map->getBoss(playerPos, true);
							respawn(map);
							Audio::getAudio().playSound(SoundID::player_hurt, false);
							Audio::getAudio().playSound(SoundID::player_resurrected, false); //player_resurrected
						}
						Audio::getAudio().playSound(SoundID::player_hurt, false);
					}
						
					Debug::DebugOutput("Ow, I'm hit! HP remaining is %d\n", HP);
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
						switch (GameConfig::get().configDifficulty)
						{
						case GameConfig::Casual:
							shield -= 1;
							break;

						case GameConfig::Hardcore:
							shield -= 1;
							break;

						case GameConfig::DmonInHell:
							shield -= 2;
							break;
						}

						Audio::getAudio().playSound(SoundID::player_shield_force, false); //player_shield_force
					}
	
				}
				else
				{
					gui->showNewUpgrade(0);
					Debug::DebugOutput("I'm fucking dead!\n");
					map->getBoss(playerPos, true);
					respawn(map);
					Audio::getAudio().playSound(SoundID::player_hurt, false);
					Audio::getAudio().playSound(SoundID::player_resurrected, false); //player_resurrected
				}
				if (result.z > 5.0f)
				{
					execute(map);
				}
			}
		}
		else
		{
			if (currentRune == MiscID::rune_shield)
			{
				effectVisible = true;
				runeEffect->getEffect()->setSpawn(playerPos.x, playerPos.y, playerPos.z);
				//runeEffect->posX = playerPos.x;
				//runeEffect->posY = playerPos.y;
				//runeEffect->posZ = playerPos.z;
			}
			invulnTimer -= 1.0f*deltaTime;
		}
	}
	if (!userInput->getKeyState('W') && !pad->jump(axesY))
		noAutoJump = true;

	//Attacking
	if (!isAttacking 
		&& (userInput->getSpace() || pad->isButtonPressed(pad->getButtons().X))
		&& flinchTimer < FLT_EPSILON)
	{
		isAttacking = true;
		attackTimer = 1.0f;
		if (currentRune == MiscID::rune_damage)
			Audio::getAudio().playSound(SoundID::player_attack_ice, false); //player_attack_ice
		else if (currentRune == MiscID::rune_range)
			Audio::getAudio().playSound(SoundID::player_attack_fire, false); //player_attack_fire
		else
			Audio::getAudio().playSound(SoundID::player_attack_miss, false); //player_attack_miss
	}

	if (isAttacking)
	{
		if (currentRune == MiscID::rune_damage || currentRune == MiscID::rune_range)
			effectVisible = true;
		animState = "attack";
		moveWeapon();
		map->attackEnemies(&attackRect, playerPos, DMG);
		attackTimer -= 2.0f*deltaTime;
		if (attackTimer < FLT_EPSILON)
		{
			isAttacking = false;
		}
	}

	if (runeEffect->getEffect())
		if (effectVisible)
		{
			runeEffect->getEffect()->update();
		}
		else
		{
			if (runeEffect->getEffect()->isFading())
				runeEffect->getEffect()->fade();
		}

	if (userInput->getKeyState('W') || pad->climb(axesY)) //MIRRORWALKING MÅSTE SKE SIST
	{
		if (progressMeter.ghostboss)
		{
			glm::vec3 val = map->mirrorWalk(collideRect, readPos());
			if (val.z > -FLT_EPSILON && teleportTime == -1.0f)
			{
				teleportPos = val;
				teleportTime = 10.0f;
				Audio::getAudio().playSound(SoundID::player_mirror_walk, false);
				return 5;
			}
		}
	}

	if (teleportTime < 0.0f && teleportTime > -1.0f)
	{
		moveTo(teleportPos.x, teleportPos.y);
		speed.x = 0.0f;
		speed.y = 0.0f;
		teleportTime = -1.0f;
		return 6;
	}
	else if(teleportTime > 0.0f)
	{
		teleportTime -= 0.1f;
	}


	if (flinchTimer > FLT_EPSILON)
		flinchTimer -= 1.0f*deltaTime;
	return 0;
}

vec2 Player::getSpeed()
{
	return speed;
}

vec3 Player::getTelePos()
{
	return teleportPos;
}

bool Player::isBossFighting()
{
	return bossFighting;
}

void Player::respawn(Map* map)
{
	HP = MAX_HP;
	shield = 0;
	speed = vec2(0);
	jumping = false;
	doubleJump = false;
	if (currentSpawn != 0 && GameConfig::get().configDifficulty != GameConfig::DmonInHell)
	{
		moveTo(currentSpawn->getPos().x, currentSpawn->getPos().y);
		Debug::DebugOutput("Jag hade en respawnpunkt\n");
	}
	else
	{
		moveTo(start.x, start.y);
	}
	map->playerDiedSoRespawnEnemies();
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
	if (progressMeter.batboss && progressMeter.spiderboss && progressMeter.ghostboss)
	{
		flinchTimer = 4.0f;
		panningToMirror = 0;
	}
	
}

Light* Player::getRuneLight(int &nrLight) const
{
	if (runeEffect->getEffect() && runeEffect->getEffect()->isFading())
		return runeEffect->getEffect()->getLights(nrLight);
	return 0;
}

bool Player::isBossDead(std::string boss)
{
	if (boss == "Bossbat")
		return progressMeter.batboss;
	if (boss == "Bossspider")
		return progressMeter.spiderboss;
	if (boss == "Bossghost")
		return progressMeter.ghostboss;
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
	Debug::DebugOutput("Fick en piece of heart, nu har jag %d\n", pickUps);
	if (pickUps == 3)
	{
		pickUps = 0;
		MAX_HP++;
		Audio::getAudio().playSound(SoundID::item_hearth_completed, false); //item_hearth_completed
		Debug::DebugOutput("Hittat 3 pickups, nu är nrOfPickups %d och nya MAX_HP är %d\n", pickUps, MAX_HP);
	}
	else
		Audio::getAudio().playSound(SoundID::item_hearth_piece, false); //item_hearth_piece
	HP = MAX_HP;
	Debug::DebugOutput("HP sattes till %d\n", HP);
	progressMeter.addHealth(chunkIndex);
}

void Player::playerGuiInfo(int* inhp, int* pieces, int* inrune, int* inshield)
{
	*inhp = HP;
	*pieces = pickUps;
	*inrune = currentRune;
	*inshield = shield;
}