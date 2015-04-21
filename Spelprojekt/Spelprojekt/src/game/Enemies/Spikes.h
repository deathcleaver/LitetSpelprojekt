#ifndef SPIKES_H
#define SPIKES_H

#include "../Enemy.h"

class Spikes : public Enemy
{
private:
public:
	Spikes(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string isBoss();
};

#endif