#ifndef GRIMSCYTHE_H
#define GRIMSCYTHE_H

#include "../Enemy.h"

class GrimScythe : public Enemy
{
private:
	float speed;
	int state;
	float stateTimer;

	bool backwards;
	float soundTimer = 0;

	int headingTo;
	glm::vec2 currentGoal;
	glm::vec2 dirToFly;
	bool reachedDestination();
	void calcDir(int position);
public:
	GrimScythe(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);

	std::string getType()
	{
		return "GrimScythe";
	}

	void returnToMaster();
};

#endif