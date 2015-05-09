#ifndef WEBSHOT_H
#define WEBSHOT_H

#include "../Enemy.h"

class Webshot : public Enemy
{
private:
	glm::vec2 direction;
	float speed;
public:
	Webshot(glm::vec2 firstPos);
	Webshot(Webshot* copy);
	void init();
	void setDirection(glm::vec2 dir);
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Webshot";
	}
};

#endif