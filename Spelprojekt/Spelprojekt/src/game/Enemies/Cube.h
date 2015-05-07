#ifndef CUBE_H
#define CUBE_H

#include "../Enemy.h"

class Cube : public Enemy
{
private:
	Rect* hurtRect;
	float acc;
	glm::vec2 speed;
	glm::vec2 maxSpeed;

	float invulnTimer;
	float jumpTimer;
public:
	~Cube();
	Cube(glm::vec2 firstPos);
	Cube(Cube* copy);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Cube";
	}
	Rect* getHurtRekt()
	{
		return hurtRect;
	}
};

#endif