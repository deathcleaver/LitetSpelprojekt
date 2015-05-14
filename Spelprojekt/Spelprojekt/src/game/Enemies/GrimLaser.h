#ifndef GRIMLASER_H
#define GRIMLASER_H

#include "../Enemy.h"

class GrimLaser : public Enemy
{
private:
	float deathTimer;
	bool vertical;
public:
	GrimLaser(glm::vec2 firstPos, bool vert);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);

	std::string getType()
	{
		if (vertical)
			return "GrimLaserV";
		else
			return "GrimLaserH";
	}
};

#endif