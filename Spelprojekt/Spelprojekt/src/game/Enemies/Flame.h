#ifndef FLAME_H
#define FLAME_H

#include "../Enemy.h"
#include "../Torch.h"

class Flame : public Enemy
{
private:
	glm::vec2 speed;
	bool collidesWithWorld(Map* map);

	float invulnTimer;
	bool flying;
	Light* myLight;
	Torch* flameEffect;

	float maxInt, minInt, increase;

	bool fading;

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
		nrLight = 10;
		return flameEffect->getLights(nrLight);
	}
};

#endif