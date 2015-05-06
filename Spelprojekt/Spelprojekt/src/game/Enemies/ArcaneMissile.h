#ifndef ARCANEMISSILE_H
#define ARCANEMISSILE_H

#include "../Enemy.h";
#include "../Effect/Effect.h"

class ArcaneMissile : public Enemy
{
private:
	glm::vec2 direction;
	float speed;
public:
	ArcaneMissile(glm::vec2 firstPos);
	ArcaneMissile(ArcaneMissile* copy);
	void init();
	void setDirection(glm::vec2 dir);
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Missile";
	}
};

#endif