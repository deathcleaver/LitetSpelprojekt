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
	GameObject* object;
	Rect* collision;
public:
	Shrine(GameObject* object);
	~Shrine();
	Rect* getRekt();
	glm::vec2 getPos();
	int getRune();
	GameObject* returnThis()
	{
		return object;
	}
};

#endif