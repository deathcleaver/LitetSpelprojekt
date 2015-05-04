#ifndef BOSSSPIDER_H
#define BOSSSPIDER_H

#include "../Enemy.h";

class Bossspider : public Enemy
{
private:
	glm::vec2 speed;
	float invulnTimer;
	bool isInit;
	int currentMode; //Similar to a state machine. -1 = Spawning. 0 = Dropping from ceiling. 1 = Running on ground. 2 = Jumping back to ceiling. 3 = HDISW.
	float modeTimer;
	float webTimer;
	int websToShoot;

	void howDoIShotWeb(glm::vec3 playerPos, Map* map);
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