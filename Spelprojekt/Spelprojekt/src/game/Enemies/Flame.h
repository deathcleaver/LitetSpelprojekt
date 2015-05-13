#ifndef FLAME_H
#define FLAME_H

#include "../Enemy.h"
#include "../Effect/Effect.h"

class Flame : public Enemy
{
private:
	glm::vec2 speed;

	float invulnTimer;
	float eruptCooldown; // DmonInHell mode only
	bool flying;
	Effect* flameEffect;
	bool fading;

	void erupt(Map* map, glm::vec3 playerPos);

public:
	~Flame();
	Flame(Flame* copy);
	Flame(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);

	bool isFading();
	void fade();

	std::string getType()
	{
		return "Flame";
	}

	Light* getLight(int &nrLight)
	{
		return flameEffect->getEffect()->getLights(nrLight);
	}
};

#endif