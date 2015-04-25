#ifndef SHRINE_H
#define SHRINE_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "gameObject.h"
#include "rect.h"
#include "Light.h"

struct RuneLights
{
	Light flameRune;
	Light sparkRune;
	Light forceRune;
	RuneLights()
	{
		flameRune.posX = 0.0f;
		flameRune.posY = 0.0f;
		flameRune.posZ = 1.0f;

		flameRune.r = 0.8f;
		flameRune.g = 0.2f;
		flameRune.b = 0.1f;
		
		flameRune.intensity = 3.0f;
		flameRune.distance = 5.0f;
		flameRune.volume = 1;

		sparkRune.posX = 0.0f;
		sparkRune.posY = 0.0f;
		sparkRune.posZ = 1.0f;

		sparkRune.r = 0.1f;
		sparkRune.g = 0.7f;
		sparkRune.b = 0.7f;

		sparkRune.intensity = 3.0f;
		sparkRune.distance = 4.0f;
		sparkRune.volume = 1;

		forceRune.posX = 0.0f;
		forceRune.posY = 0.0f;
		forceRune.posZ = 1.0f;

		forceRune.r = 0.1f;
		forceRune.g = 0.9f;
		forceRune.b = 0.1f;

		forceRune.intensity = 3.0f;
		forceRune.distance = 5.0f;
		forceRune.volume = 1;
	}
};

class Shrine
{
private:
	//Powerup
	MiscID rune;
	bool runeTaken;
	GameObject* object;
	GameObject* runeObj;
	Rect* collision;

	float runeMove;
	bool runeUp;

	Light* myLight;
public:
	RuneLights* lightForPlayer;
	Shrine(GameObject* object, MiscID runetype);
	~Shrine();
	void giveLight(Light* l);
	Rect* getRekt();
	glm::vec2 getPos();
	MiscID getRune();
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