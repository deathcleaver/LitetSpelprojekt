#ifndef PLAYER_H
#define PLAYER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "Shrine.h"
#include "rect.h"
#include "map.h"
#include "mapChunk.h"
#include "../UserInput.h"

class Player : public GameObject
{
private:
	float timepass = 0.0f;
	int MAX_HP;
	int HP;

	//movement
	float jumpHeight;
	vec2 speed;
	vec2 maxSpeed;
	vec2 acceleration;
	bool jumping;
	bool noAutoJump;
	float landBreak;

	//attacks
	bool facingRight;
	bool isAttacking;
	float attackTimer;
	Rect attackRect;
	mat4 weaponMatrix;
	void moveWeapon();

	float flinchTimer;
	float invulnTimer;

	Rect* collideRect = 0;
	Shrine* currentSpawn = 0;
	bool god;
	bool noclip;

	void respawn(Map* map);

	std::string animState = "";
public:
	Player(){};
	~Player();
	void init();

	int update(UserInput* userInput, Map* map, float deltaTime);
	vec2 getSpeed();
	std::string getAnimState();
	bool isBlinking() const;

	void execute(Map* map);
};

#endif