#ifndef WEB_H
#define WEB_H

#include "enemy.h"

class Web : public Enemy
{
private:
public:
	Web(glm::vec2 firstPos)
	{
		initPos = firstPos;
		moveTo(initPos.x, initPos.y);
		collideRect = new Rect();
		collideRect->initGameObjectRect(&worldMat, 1, 1);
		contentIndex = 6;
	}
	void init()
	{
		moveTo(initPos.x, initPos.y);
	}
	int update(float deltaTime, Map* map, glm::vec3 playerPos)
	{
		return 0;
	}
	void hit(int damage, bool playerRightOfEnemy)
	{
	}
	std::string getType()
	{
		return "Web";
	}
};

#endif