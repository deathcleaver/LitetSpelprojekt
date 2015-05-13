#ifndef FIREBALL_H
#define FIREBALL_H

#include "../Enemy.h"
#include "../Effect/Effect.h"

class Fireball : public Enemy
{
private:
	glm::vec2 direction;
	float speed;
	float deathTimer;

	Effect* flameEffect;
public:
	Fireball(glm::vec2 firstPos);
	Fireball(Fireball* copy);
	~Fireball();
	void init();
	void setDirection(glm::vec2 dir);
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Missile";
	}

	Light* getLight(int &nrLights)
	{
		return flameEffect->getEffect()->getLights(nrLights);
	}

	void fade()
	{
		flameEffect->getEffect()->fade();
	}

	bool isFading()
	{
		return flameEffect->getEffect()->isFading();
	}
};

#endif