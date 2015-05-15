#ifndef ECHOLOCATION_H
#define ECHOLOCATION_H

#include "../Enemy.h"

class EchoLocation : public Enemy
{
private:
	float deathTimer;
	glm::vec2 direction;
public:
	EchoLocation(glm::vec2 firstPos, glm::vec2 dir);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);

	std::string getType()
	{
		return "EchoLocation";
	}

	glm::vec2 getDirection()
	{
		return direction;
	}
};

#endif