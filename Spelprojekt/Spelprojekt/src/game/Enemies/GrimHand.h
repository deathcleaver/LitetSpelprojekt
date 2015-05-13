#ifndef GRIMHAND_H
#define GRIMHAND_H

#include "../Enemy.h"

class GrimHand : public Enemy
{
private:
	int grimMode;
	float fireTimer;

	int state;
	float invulnTimer;
	float stateTimer;

	float clapTimer;
	int leftHandSwingCounter;

	glm::vec2 speed;

	glm::vec2 neutralPos;
	void leftHandState(float deltaTime, Map* map, glm::vec3 playerPos);

	void rightHandState(float deltaTime, Map* map, glm::vec3 playerPos);

	void GrimHand::calcDir(glm::vec2 destination);
	bool GrimHand::reachedDestination();
	glm::vec2 dirToFly;
	glm::vec2 currentGoal;

	void fireBall(Map* map, glm::vec3 playerPos);
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

	void setGrimMode(int m)
	{
		grimMode = m;
	}
};

#endif