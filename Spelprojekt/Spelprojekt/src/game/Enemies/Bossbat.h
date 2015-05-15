#ifndef BOSSBAT_H
#define BOSSBAT_H

#include "../Enemy.h"

class Bossbat : public Enemy
{
private:
	Rect* hurtRect;

	float movementScale;
	float speed;
	bool slow;
	float invulnTimer;
	bool isInit;

	float batTimer;
	int spawnsLeft;
	void spawnBat(Map* map, float deltaTime);
	void echoLocation(Map* map, glm::vec3 playerPos);
	float echoTimer;
	bool isShootingEchos;

	float chargeTimer;
	bool charging;
	glm::vec3 chargePos;
	glm::vec3 returnPos;
	bool hasTurned;
public:
	~Bossbat();
	Bossbat(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	bool isBlinking();
	bool isInitiated();
	std::string getType()
	{
		return "Bossbat";
	}

	glm::vec2 plsGiveBossPos();

	Rect* getHurtRekt()
	{
		if (!charging)
			return hurtRect;
		return collideRect;
	}
};

#endif