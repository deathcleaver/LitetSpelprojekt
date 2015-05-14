#ifndef GRIM_H
#define GRIM_H

#include "../Enemy.h"

class Grim : public Enemy
{
private:
	int mode; //0 = start, 1 = force, 2 = range, 3 = spark, 4 = dying, 5 = reaper

	int state;
	float stateTimer;
	float speed;

	float fireBallTimer;

	Rect* hurtRect;
	float invulnTimer;
	bool isInit;

	void fireBall(Map* map, glm::vec3 playerPos, bool rightEye);

	void calcDir(int position);
	bool reachedDestination();
	glm::vec2 dirToFly;
	glm::vec2  currentGoal;
	int headingTo;
public:
	~Grim();
	Grim(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	bool isBlinking();
	bool isInitiated();
	std::string getType()
	{
		return "Grim";
	}

	glm::vec2 plsGiveBossPos();

	Rect* getHurtRekt()
	{
		if (state != -1)
			return hurtRect;
		return 0;
	}

	glm::vec2 getHandPos();

	Rect* getRekt();
};

#endif