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
#include "../Gamepad.h"
#include "Progress.h"
#include "Effect/Effect.h"

class GUI;

class Player : public GameObject
{
private:
	int panningToMirror = -1;

	bool gamePadButtonPressed(char keyBoardButton);

	Progress progressMeter;
	float timepass = 0.0f;
	int MAX_HP;
	int HP;
	int currentRune;
	Effect* runeEffect;
	//Light* runeEffect;
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

	int pickUps = 0;

	glm::vec2 start;
public:
	Player(){};
	~Player();

	void init();
	void setStartPos(int x, int y);
	int update(UserInput* userInput, Gamepad* pad, Map* map, GUI* gui, float deltaTime);
	vec2 getSpeed();
	std::string getAnimState();
	bool isBlinking() const;
	void playerGuiInfo(int* hp, int* pieces, int* rune, int* shield);
	void execute(Map* map);

	void fightThatBossBro();
	void dingDongTheBossIsDead(std::string boss);
	bool isBossFighting();

	Light* getRuneLight(int &nrLight) const;

	Shrine* getCurrentSpawn()
	{
		return currentSpawn;
	}

	bool isBossDead(std::string boss);
	void setProgress(Progress p);
	Progress getProgress();

	void getPickup(glm::vec2 chunkIndex);
};

#endif