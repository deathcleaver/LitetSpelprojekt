#ifndef BAT_H
#define BAT_H

#include "../Enemy.h";

class Bat : public Enemy
{
private:
	int wingsUpIndex;
	float interpolation;
public:
	Bat(glm::vec2 firstPos);
	void init();
	int update(float deltaTime);
	void hit(int damage);
	void getContentIndices(int &state1, int &state2, float &interpol);
};

#endif