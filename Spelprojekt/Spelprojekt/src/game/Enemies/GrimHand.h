#ifndef GRIMHAND_H
#define GRIMHAND_H

#include "../Enemy.h"

class GrimHand : public Enemy
{
private:
	int grimMode;

	int state;
	float invulnTimer;
	float stateTimer;

	float clapTimer;

	glm::vec2 speed;

	void leftHandState(int state, float deltaTime, Map* map, glm::vec3 playerPos);

	void rightHandState(int state, float deltaTime, Map* map, glm::vec3 playerPos);

	void GrimHand::calcDir(glm::vec2 destination);
	bool GrimHand::reachedDestination();
	glm::vec2 dirToFly;
	glm::vec2 currentGoal;
public:
	GrimHand(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "GrimHand";
	}
	bool isBlinking();
};

#endif