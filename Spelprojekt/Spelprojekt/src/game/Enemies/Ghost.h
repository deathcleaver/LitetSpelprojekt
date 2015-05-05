#ifndef GHOST_H
#define GHOST_H

#include "../AudioObject.h"
#include "../Enemy.h"
#include <cstdlib>

class Ghost : public Enemy
{
private:
	float speed;
	float invulnTimer;
	AudioObject audioObj;
	glm::vec2 randdir;
public:
	Ghost(glm::vec2 firstPos);
	Ghost(Ghost* copy);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Ghost";
	}
	bool isBlinking();
};

#endif