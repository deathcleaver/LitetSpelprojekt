#ifndef CUBE_H
#define CUBE_H

#include "../Enemy.h"
#include "../Effect/Light.h"

class Cube : public Enemy
{
private:
	Rect* hurtRect;
	float acc;
	glm::vec2 speed;
	glm::vec2 maxSpeed;

	float invulnTimer;
	float jumpTimer;

	Light* glow;
	void setupGlow();

	//DmonInHell
	int initHealth;
	bool split = false; // not split yet
	bool willSplit = false;
	void getInHere(Map* map, glm::vec3 playerPos);
public:
	~Cube();
	Cube(glm::vec2 firstPos);
	Cube(glm::vec2 firstPos, bool s, float speedX, int hp);
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
	Light* getGlow()
	{
		return glow;
	}
};

#endif