#ifndef DUMMY_H
#define DUMMY_H

#include "../Enemy.h"

class Dummy : public Enemy
{
private:
public:
	Dummy(glm::vec2 firstPos)
	{
		initPos = firstPos;
		collideRect = new Rect();
		collideRect->initGameObjectRect(&worldMat, 0.001f, 0.001f);
	}
	void init()
	{
		moveTo(initPos.x, initPos.y);
		collideRect->update();
	}
	int update(float deltaTime, Map* map, glm::vec3 playerPos)
	{
		return 0;
	}
	void hit(int damage, bool playerRightOfEnemy){}
	std::string isBoss()
	{
		return "ChuckTesta";
	}
	std::string getType()
	{
		return "ChuckTesta";
	}

	glm::vec2 plsGiveBossPos()
	{
		return initPos;
	}
};

#endif