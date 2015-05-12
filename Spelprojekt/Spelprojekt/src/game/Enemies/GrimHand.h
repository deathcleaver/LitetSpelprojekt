#ifndef GRIMHAND_H
#define GRIMHAND_H

#include "../Enemy.h"

class GrimHand : public Enemy
{
private:
public:
	GrimHand(glm::vec2 firstPos);
	GrimHand(GrimHand* copy);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "GrimHand";
	}
};

#endif