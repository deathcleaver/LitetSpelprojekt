#ifndef BOSSGHOST_H
#define BOSSGHOST_H

#include "../Enemy.h";

class Bossghost : public Enemy
{
private:
	Rect* hurtRect;

	glm::vec2 dirToMid;
	float movementScale;
	float speed;
	float invulnTimer;
	bool isInit;

	bool inMirror;
	int state; //-1 = spawning. 0 = Jumping out of mirror. 2 = Flying outside of mirror. 3 = Flying into mirror. 4 = Flying inside mirror.
	float stateTimer;
public:
	~Bossghost();
	Bossghost(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	bool isBlinking();
	bool isInitiated();
	std::string getType()
	{
		return "Bossghost";
	}

	glm::vec2 plsGiveBossPos();

	Rect* getHurtRekt()
	{
		if (!inMirror)
			return hurtRect;
	}

	int getState()
	{
		return state;
	}
};

#endif