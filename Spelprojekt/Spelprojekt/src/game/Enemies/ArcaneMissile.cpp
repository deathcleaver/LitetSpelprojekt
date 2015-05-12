#include "ArcaneMissile.h"
#include "../Web.h"
#include "../map.h"
#include "../Effect/Torch.h"

ArcaneMissile::ArcaneMissile(glm::vec2 firstPos)
{
	initPos = firstPos; //This is never actually used, only copy constructor matters
	moveTo(initPos.x, initPos.y);
	flameEffect = new Effect();
	flameEffect->create(EffectType::torch);
	flameEffect->getEffect()->init(firstPos.x, firstPos.y, 0);
	flameEffect->getEffect()->setParticleColor(0.5f, 0.8f, 0.8f);
	flameEffect->getEffect()->timeChangeColor(false, false, true);
	((Torch*)flameEffect->getEffect())->setIntensity(15);
	deathTimer = 3.0f;
	audibleDistance = 10.0f;
}

ArcaneMissile::ArcaneMissile(ArcaneMissile* copy)
{
	alive = true;
	worldMat = copy->worldMat;
	deathTimer = copy->deathTimer;
	scaleFactor(0.01f, 0.01f, 0.01f);
	initPos = copy->initPos;
	moveTo(initPos.x, initPos.y);
	contentIndex = EnemyID::spikes;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.3f, 0.3f);

	health = 1;
	speed = 0.1f + 0.1f * GameConfig::get().configDifficulty;

	flameEffect = new Effect();
	flameEffect->reCreate(EffectType::torch);
	flameEffect->getEffect()->copy(copy->flameEffect->getEffect());
	((Torch*)flameEffect->getEffect())->setIntensity(15);
	
	direction = copy->direction;
}

ArcaneMissile::~ArcaneMissile()
{
	delete flameEffect;
}

void ArcaneMissile::setDirection(glm::vec2 dir)
{
	direction = dir;
}

void ArcaneMissile::init()
{
	//Not needed since it's a visitor-only projectile
}

int ArcaneMissile::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	deathTimer -= 1.0*deltaTime;
	if (deathTimer < FLT_EPSILON)
	{
		alive = false;
		return 1;
	}
	if (alive)
	{
		speed += speed*0.05f;
		translate(direction.x*speed*deltaTime, direction.y*speed*deltaTime);
		if (collidesWithWorld(map))
		{
			Audio::getAudio().playSoundAtPos(SoundID::enemy_missile_death, readPos(), audibleDistance, false);
			alive = false;
			return 1;
		}
		glm::vec3 pos = readPos();
		collideRect->update();
		flameEffect->getEffect()->setSpawn(pos.x, pos.y, pos.z);
		flameEffect->update();
		return 0;
	}
}

void ArcaneMissile::hit(int damage, bool playerRightOfEnemy)
{
	direction.x = -direction.x;
}