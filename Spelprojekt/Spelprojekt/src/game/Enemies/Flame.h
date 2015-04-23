#ifndef FLAME_H
#define FLAME_H

#include "../Enemy.h"
#include "../Light.h"

class Flame : public Enemy
{
private:
	glm::vec2 speed;
	bool collidesWithWorld(MapChunk* chunk);

	float invulnTimer;
	bool flying;
	Light* myLight;

	float maxInt, minInt, increase;

	bool fading;

public:
	~Flame();
	Flame(Flame* copy);
	Flame(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);

	bool isFading();
	void fade();

	std::string getType()
	{
		return "Flame";
	}

	Light* getLight()
	{
		return myLight;
	}
};

#endif