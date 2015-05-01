#ifndef GHOST_H
#define GHOST_H

#include "../Enemy.h"

class Ghost : public Enemy
{
private:
	float speed;
	float invulnTimer;
public:
	Ghost(glm::vec2 firstPos);
	Ghost(Ghost* copy);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Ghost";
	}
	bool isBlinking();
};

#endif