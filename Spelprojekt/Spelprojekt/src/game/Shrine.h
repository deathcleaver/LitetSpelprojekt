#ifndef SHRINE_H
#define SHRINE_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "gameObject.h"
#include "rect.h"

enum ShrineUpgrades
{
	NORUNE,
	FLAME,
	SPARK,
	FORCE,
	STOMP
};

class Shrine
{
private:
	//Powerup
	int rune;
	bool runeTaken;
	GameObject* object;
	GameObject* runeObj;
	Rect* collision;

	float runeMove;
	bool runeUp;
public:
	Shrine(GameObject* object, int runetype);
	~Shrine();
	Rect* getRekt();
	glm::vec2 getPos();
	int getRune();
	void resetRune();
	GameObject* returnThis()
	{
		return object;
	}
	GameObject* returnRune()
	{
		if (!runeTaken)
			return runeObj;
		return 0;
	}

	void update(float deltaTime);
};

#endif