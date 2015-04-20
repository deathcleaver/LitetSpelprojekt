#ifndef FLAME_H
#define FLAME_H

#include "../Enemy.h"

class Flame : public Enemy
{
private:
	float speed;
	bool collidesWithWorld(MapChunk* chunk);

	float invulnTimer;
	bool hasBounced;
public:
	Flame(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, MapChunk* chunk);
	void hit(int damage, bool playerRightOfEnemy);
};

#endif