#ifndef HEALTHPICKUP_H
#define HEALTHPICKUP_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include <string>

#include "gameObject.h"
#include "rect.h"
#include "Light.h"

class HealthPickup : public GameObject
{
protected:
	bool taken = false;
	Rect* collideRect;
	Light* myLight;
public:
	Rect* getRekt()
	{
		return collideRect;
	}
	Light* getLight()
	{
		if (!taken)
			return myLight;
		return 0;
	}
	void init(glm::vec2 pos);
	~HealthPickup();
	int update(float deltaTime);
	bool isTaken()
	{
		return taken;
	}
	void take()
	{
		taken = true;
	}
};

#endif