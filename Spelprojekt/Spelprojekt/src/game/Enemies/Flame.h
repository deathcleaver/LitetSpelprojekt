#ifndef FLAME_H
#define FLAME_H

#include "../Enemy.h"

class Flame : public Enemy
{
private:
	glm::vec2 speed;
	bool collidesWithWorld(MapChunk* chunk);

	float invulnTimer;
	bool flying;
public:
	Flame(Flame* copy);
	Flame(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Flame";
	}
};

#endif