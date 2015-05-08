#ifndef GHOST_H
#define GHOST_H

#include "../Effect/Effect.h"
#include "../AudioObject.h"
#include "../Enemy.h"
#include <cstdlib>

class Ghost : public Enemy
{
private:
	float speed;
	float invulnTimer;
	AudioObject audioObj;
	glm::vec2 randdir;

	Effect* effect;

	bool fading;
public:
	Ghost(glm::vec2 firstPos);
	Ghost(Ghost* copy);
	~Ghost();
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Ghost";
	}
	bool isBlinking();

	bool isFading()
	{
		return fading;
	}
	void fade();

	Light* getLight(int &nrLight)
	{
		return ((Spark*)(effect->getEffect()))->getLights(nrLight);
	}
};

#endif