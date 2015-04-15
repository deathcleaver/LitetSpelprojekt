#ifndef BAT_H
#define BAT_H

#include "../Enemy.h";

class Bat : public Enemy
{
private:
	float movementScale;
public:
	Bat(glm::vec2 firstPos);
	void init();
	int update(float deltaTime);
	void hit(int damage);
};

#endif