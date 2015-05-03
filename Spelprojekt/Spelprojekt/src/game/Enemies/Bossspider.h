#ifndef BOSSSPIDER_H
#define BOSSSPIDER_H

#include "../Enemy.h";

class Bossspider : public Enemy
{
private:
	float speed;
	float invulnTimer;
	bool isInit;

public:
	Bossspider(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	bool isBlinking();
	bool isInitiated();
	std::string getType()
	{
		return "Bossspider";
	}

	glm::vec2 plsGiveBossPos();
};

#endif