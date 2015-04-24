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
#include "Progress.h"

class Player : public GameObject
{
private:
	Progress progressMeter;
	float timepass = 0.0f;
	int MAX_HP;
	int HP;
	int currentRune;
	Light* runeEffect;
	bool effectVisible;

	int DMG;
	int shield;

	//movement
	float jumpHeight;
	vec2 speed;
	vec2 maxSpeed;
	vec2 acceleration;
	bool jumping;
	bool doubleJump;
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

	bool bossFighting = false;
public:
	Player(){};
	~Player();
	void init();

	int update(UserInput* userInput, Map* map, Audio* audio, float deltaTime);
	vec2 getSpeed();
	std::string getAnimState();
	bool isBlinking() const;

	void execute(Map* map);

	void fightThatBossBro();
	void dingDongTheBossIsDead(std::string boss);
	bool isBossFighting();

	Light* getRuneLight() const;

	Shrine* getCurrentSpawn()
	{
		return currentSpawn;
	}

	bool isBossDead(std::string boss);
	void setProgress(Progress p);
	Progress getProgress();
};

#endif