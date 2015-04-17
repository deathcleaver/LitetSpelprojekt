#ifndef SHRINE_H
#define SHRINE_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "gameObject.h"
#include "rect.h"

class Shrine
{
private:
	//Powerup
	int powerUp;
	GameObject* object;
	Rect* collision;
public:
	Shrine(GameObject* object);
	~Shrine();
	Rect* getRekt();
	glm::vec2 getPos();
	//Give Powerup
};

#endif