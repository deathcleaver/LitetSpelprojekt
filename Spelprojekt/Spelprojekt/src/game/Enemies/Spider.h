#ifndef SPIDER_H
#define SPIDER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include <string>
#include "../enemy.h"

class Spider : public Enemy
{
private:
	bool ceiling;
	bool jumping;
	bool facingRight;
	float jumpTimer;

	glm::vec2 speed;
public:
	Spider(glm::vec2 firstPos);
	Spider(Spider* copy);
	~Spider();
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Spider";
	}
};

#endif