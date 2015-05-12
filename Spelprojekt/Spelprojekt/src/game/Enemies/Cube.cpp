#include "Cube.h"
#include "../map.h"

Cube::Cube(glm::vec2 firstPos)
{
	float tempScale = 1.8f;
	float tempCollide = 1.8f;
	float tempHurt = 2.1f;
	health = 2;
	if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
	{
		tempCollide *= 1.2f;
		tempScale *= 1.6f;
		tempHurt *= 1.6f;
		health = 4;
	}

	scaleFactor(tempScale, tempScale, tempScale);
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = EnemyID::cube;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, tempCollide, tempCollide);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, tempHurt, tempHurt);

	audibleDistance = 3.0f;

	speed = glm::vec2(0,0);
	maxSpeed = glm::vec2(12, 30);
	maxSpeed.x += GameConfig::get().configDifficulty * 2; // + (0->2);
	acc = 0.3f;
	invulnTimer = 0.0f;
	jumpTimer = 0.0f;

	glow = new Light();
	setupGlow();
}

Cube::~Cube()
{
	delete hurtRect;
	delete glow;
}

void Cube::setupGlow()
{
	glm::vec3 cubePos = readPos();
	glow->posX = cubePos.x; glow->posY = cubePos.y; glow->posZ = cubePos.z;

	for (int c = 0; c < 4; c++)
	{
		glow->r = 0.02f;
		glow->g = 0.8f;
		glow->b = 0.2f;
		glow->distance = 8.0f;
		glow->intensity = 1.5f;
		glow->volume = 0;
	}
}

Cube::Cube(Cube* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	glm::vec3 copyPos = copy->readPos();
	initPos = glm::vec2(copyPos.x, copyPos.y);
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = copy->facingRight;
	contentIndex = EnemyID::cube;
	health = copy->health;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1.8f, 1.8f);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 2.1f, 2.1f);

	speed = copy->speed;
	maxSpeed = glm::vec2(12, 30);
	maxSpeed.x = copy->maxSpeed.x;
	acc = 0.3f;
	invulnTimer = copy->invulnTimer;

	glow = new Light();
	setupGlow();
}

void Cube::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
		health = 4;
	else
		health = 2;

	collideRect->update();
}

int Cube::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec3 myPos = readPos();
	if (playerPos.x < myPos.x)
	{
		speed.x -= acc;
		if (speed.x < 0.0)
			facingRight = false;
		if (speed.x < -maxSpeed.x && invulnTimer < FLT_EPSILON)
			speed.x = -maxSpeed.x;
		moveTo(myPos.x + speed.x*deltaTime, myPos.y);
		if (collidesWithWorld(map))
		{
			if (speed.x < -2.0f)
			{
				if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
					Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false, 0.6f); //enemy_slime_jump
				else
					Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false); //enemy_slime_jump
				
			}
				

			moveTo(myPos.x - speed.x*deltaTime, myPos.y);
			speed.x = 0;
			jumpTimer += 1.0f*deltaTime;
		}
	}
	else if (playerPos.x > myPos.x)
	{
		speed.x += acc;
		if (speed.x > 0.0)
			facingRight = true;
		if (speed.x > maxSpeed.x && invulnTimer < FLT_EPSILON)
			speed.x = maxSpeed.x;
		moveTo(myPos.x + speed.x*deltaTime, myPos.y);
		if (collidesWithWorld(map))
		{
			if (speed.x > 2.0f)
			{
				if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
					Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false, 0.6f); //enemy_slime_jump
				else
					Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false); //enemy_slime_jump
			}

			moveTo(myPos.x - speed.x*deltaTime, myPos.y);
			speed.x = 0;
			jumpTimer += 1.0f*deltaTime;
		}
	}
	myPos = readPos();
	speed.y += 0.7f;
	if (speed.y > maxSpeed.y)
		speed.y = maxSpeed.y;
	moveTo(myPos.x, myPos.y - speed.y*deltaTime);
	if (collidesWithWorld(map))
	{
		if (speed.y > 5.0f || speed.y < -5.0f)
		{
			if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
				Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false, 0.6f); //enemy_slime_jump
			else
				Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false); //enemy_slime_jump
		}

		moveTo(myPos.x, myPos.y + speed.y*deltaTime);
		speed.y = 0;
	}

	if (jumpTimer > 1.0f)
	{
		speed.y -= 20.0f;
		jumpTimer = 0.0f;
		if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
			Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false, 0.6f); //enemy_slime_jump
		else
			Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_jump, myPos, audibleDistance, false); //enemy_slime_jump
	}

	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
	}
	hurtRect->update();

	glm::vec3 cubePos = readPos();
	glow->posX = cubePos.x;
	glow->posY = cubePos.y;
	glow->posZ = cubePos.z;
	return 0;
}

void Cube::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (playerRightOfEnemy)
			speed.x = -16.0f;
		else
			speed.x = 16.0f;
		if (health <= 0)
		{
			alive = false;
			if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
				Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_death, readPos(), audibleDistance, false, 0.6f); //enemy_slime_death
			else
				Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_death, readPos(), audibleDistance, false); //enemy_slime_death
		}
		else
		{
			if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
				Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_hurt, readPos(), audibleDistance, false, 0.6f); //enemy_slime_death
			else
				Audio::getAudio().playSoundAtPos(SoundID::enemy_slime_hurt, readPos(), audibleDistance, false); //enemy_slime_death
		}

		invulnTimer = 0.6f;
	}

}