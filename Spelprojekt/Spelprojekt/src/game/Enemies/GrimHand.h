#ifndef GRIMHAND_H
#define GRIMHAND_H

#include "../Enemy.h"

static float clapTimer;

class GrimHand : public Enemy
{
private:
	Rect* hurtRect;

	int grimMode;
	float fireTimer;

	int state;
	float invulnTimer;
	float stateTimer;
	bool stunned;
	bool clapSound = true;

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
	~GrimHand()
	{
		delete hurtRect;
	}
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

	Rect* getHurtRekt()
	{
		return hurtRect;
	}

	Rect* getRekt()
	{
		if (invulnTimer < FLT_EPSILON)
			return collideRect;
		return 0;
	}
};

#endif