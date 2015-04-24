#ifndef CUBE_H
#define CUBE_H

#include "../Enemy.h"

class Cube : public Enemy
{
private:
	float acc;
	glm::vec2 speed;
	glm::vec2 maxSpeed;

	float invulnTimer;
public:
	Cube(glm::vec2 firstPos);
	Cube(Cube* copy);
	void init();
	int update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Cube";
	}

	bool collidesWithWorld(MapChunk* chunk);
};

#endif