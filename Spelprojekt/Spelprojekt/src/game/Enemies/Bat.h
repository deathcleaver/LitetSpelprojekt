#ifndef BAT_H
#define BAT_H

#include "../Enemy.h";

class Bat : public Enemy
{
private:
	float movementScale;
	float speed;
	bool collidesWithWorld(MapChunk* chunk);
	bool slow;
public:
	Bat(glm::vec2 firstPos);
	Bat(Bat* copy);
	void init();
	int update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	void setFacing(bool faceRight);
	std::string getType()
	{
		return "Bat";
	}
};

#endif