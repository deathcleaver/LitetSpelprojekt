#ifndef BOSSGHOST_H
#define BOSSGHOST_H

#include "../Enemy.h"

class Bossghost : public Enemy
{
private:
	Rect* hurtRect;

	float topMidBot[3];
	float leftRight[2];
	glm::vec2 dirToFly;
	glm::vec2 currentGoal;
	float movementScale;
	float speed;
	float mirrorSpeed;
	float invulnTimer;
	bool isInit;

	bool hasBeenHit;
	bool inMirror;
	int state; //-1 = spawning. 0 = Jumping out of mirror. 2 = Flying outside of mirror. 3 = Missile Barrage. 4 = Flying into mirror. 5 = Flying inside mirror.
	float stateTimer;
	float ghostTimer;
	bool isDying;
	int lastPos;

	void calcDir(int posOutOfMirror);
	bool reachedDestination();
	void shootMissile(glm::vec3 playerPos, Map* map, bool followPlayer);
	void getSpooky(Map* map);
	void getNerdy(Map* map);
	int missilesLeft;
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
		return 0;
	}

	int getState()
	{
		return state;
	}

	Rect* getRekt()
	{
		if (!inMirror && invulnTimer < FLT_EPSILON)
			return collideRect;
		return 0;
	}
};

#endif