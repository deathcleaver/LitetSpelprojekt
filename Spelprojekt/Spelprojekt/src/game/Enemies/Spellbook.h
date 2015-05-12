#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "../Enemy.h"

class Spellbook : public Enemy
{
private:
	float speed;
	float invulnTimer;
	int yMovement = 1;
	float spellCooldown;
	int minionCount = 0, minionsMax = 3;
	void castSpell(Map* map, glm::vec3 playerPos);
	void spellSummonGhost(Map* map, glm::vec3 playerPos);
	void spellArcaneMissile(Map* map, glm::vec3 playerPos);

public:
	Spellbook(glm::vec2 firstPos);
	Spellbook(Spellbook* copy);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	std::string getType()
	{
		return "Spellbook";
	}

	Rect* getRekt()
	{
		if (invulnTimer < FLT_EPSILON)
			return collideRect;
		return 0;
	}
};

#endif