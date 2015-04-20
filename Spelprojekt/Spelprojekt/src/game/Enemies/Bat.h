#ifndef BAT_H
#define BAT_H

#include "../Enemy.h";

class Bat : public Enemy
{
private:
	float movementScale;
	float speed;
	bool collidesWithWorld(MapChunk* chunk);
public:
	Bat(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, MapChunk* chunk);
	void hit(int damage, bool playerRightOfEnemy);
};

#endif