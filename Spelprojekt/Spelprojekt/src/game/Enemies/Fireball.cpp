#include "Fireball.h"
#include "../Web.h"
#include "../map.h"
#include "../Effect/Torch.h"

Fireball::Fireball(glm::vec2 firstPos)
{
	initPos = firstPos; //This is never actually used, only copy constructor matters
	moveTo(initPos.x, initPos.y);
	flameEffect = new Effect();
	flameEffect->create(EffectType::torch);
	flameEffect->getEffect()->init(firstPos.x, firstPos.y, 0);
	flameEffect->getEffect()->setParticleColor(0.8f, 0.8f, 0.1f);
	flameEffect->getEffect()->timeChangeColor(false, false, true);
	((Torch*)flameEffect->getEffect())->setIntensity(15);
	deathTimer = 3.0f;
	audibleDistance = 10.0f;
}

Fireball::Fireball(Fireball* copy)
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
	speed = 0.01f;

	flameEffect = new Effect();
	flameEffect->reCreate(EffectType::torch);
	flameEffect->getEffect()->copy(copy->flameEffect->getEffect());
	((Torch*)flameEffect->getEffect())->setIntensity(15);
	
	direction = copy->direction;
}

Fireball::~Fireball()
{
	delete flameEffect;
}

void Fireball::setDirection(glm::vec2 dir)
{
	direction = dir;
}

void Fireball::init()
{
	//Not needed since it's a visitor-only projectile
}

int Fireball::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	deathTimer -= 1.0*deltaTime;
	if (deathTimer < FLT_EPSILON)
	{
		alive = false;
		return 1;
	}
	if (alive)
	{
		speed -= 0.2f;
		translate(0, 0);
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

void Fireball::hit(int damage, bool playerRightOfEnemy)
{
	direction.x = -direction.x;
}