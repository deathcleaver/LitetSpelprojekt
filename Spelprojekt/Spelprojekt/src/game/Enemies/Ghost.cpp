#include "Ghost.h"
#include "../mapChunk.h"
#include "../Effect/BaseEffect.h"

Ghost::Ghost(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 5;
	health = 3;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);

	speed = 2.0f;

	effect = new Effect();
	effect->create(EffectType::spark);
	effect->getEffect()->init(firstPos.x, firstPos.y, 0);
	((Spark*)(effect))->setIntensity(2);
}

Ghost::Ghost(Ghost* copy)
{
	visitor = copy->visitor;
	worldMat = copy->worldMat;
	initPos = glm::vec2(readPos());
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = true;
	contentIndex = 5;
	health = copy->health;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);

	speed = 3.0f;

	effect = new Effect();
	effect->reCreate(EffectType::spark);
	effect->getEffect()->copy(copy->effect->getEffect());
}

void Ghost::init()
{
	moveTo(initPos.x, initPos.y);
	facingRight = true;
	alive = true;
	health = 3;

	// init audioobj that holds the moan sounds
	audioObj.init(true, 30, glm::vec3(initPos.x, initPos.y, 0), 3.5f, 1, 0, 0); //enemy_ghost_moan
	//audioObj.update(0);
	//audioObj.setPitch(0.9f);
	//audioObj.setPosition(readPos());

	collideRect->update();

	effect->getEffect()->init(initPos.x, initPos.y, 0);

}

int Ghost::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	glm::vec2 pos = glm::vec2(readPos().x, readPos().y);
	glm::vec2 pPos = glm::vec2(playerPos.x, playerPos.y);
	if (invulnTimer < FLT_EPSILON)
	{
		glm::vec2 dist = pos - pPos;
		float distance = sqrt(dist.x*dist.x + dist.y*dist.y);

		if (distance < 10.0f)
		{
			glm::vec2 normDist = normalize(dist);
			float xSpeed = -normDist.x*speed;
			float ySpeed = -normDist.y*speed;
			moveTo(pos.x + xSpeed*deltaTime, pos.y + ySpeed*deltaTime);
		}
		collideRect->update();
	}
	else
	{
		moveTo(pos.x + randdir.x*deltaTime*2.0f, pos.y + randdir.y*deltaTime*2.0f);
		invulnTimer -= 1.0f*deltaTime;
	}


	// update ghost_moan (wip)
	//audioObj.update(deltaTime);
	//audioObj.setPosition(glm::vec3(pos.x, pos.y, 0.0f));

	effect->update();
	effect->getEffect()->setSpawn(pos.x, pos.y, 0);

	return 0;
}

void Ghost::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health <= 0)
		{
			alive = false;
			audioObj.stopSound();
			Audio::getAudio().playSoundAtPos(SoundID::enemy_ghost_death, readPos(), 2.5, false); //enemy_ghost_death
		}
		else
		{
			Audio::getAudio().playSoundAtPos(SoundID::enemy_ghost_hurt, readPos(), 2.5, false); //enemy_ghost_hurt
			invulnTimer = 2.0f;
			float direction = rand() % 360 * 0.0174532925f;
			randdir.x = cos(direction);
			randdir.y = sin(direction);
			randdir = normalize(randdir);
		}
	}
}

bool Ghost::isBlinking()
{
	if (invulnTimer > FLT_EPSILON)
		return true;
	return false;
}

void Ghost::fade()
{
	((Spark*)effect->getEffect())->fade();
	fading = ((Spark*)effect->getEffect())->isFading();
}