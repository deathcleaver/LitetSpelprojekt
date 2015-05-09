#ifndef DEATHBOX_H
#define DEATHBOX_H

#include "../Enemy.h"

class Deathbox : public Enemy
{
private:
public:
	Deathbox(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Deathbox";
	}
};

#endif