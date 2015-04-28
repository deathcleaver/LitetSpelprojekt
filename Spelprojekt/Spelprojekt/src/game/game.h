#ifndef GAME_H
#define GAME_H

#include "../UserInput.h"
#include "../GUI.h"

#include "edit.h"
#include "../engine/engine.h"
#include "../engine/ContentManager.h"
#include "../engine/object.h"
#include "../engine/ObjectManager.h"

//#include "audio"
#include "player.h"
#include "enemyManager.h"
#include "map.h"

#include <GLFW/glfw3.h>

enum gameState
{
	MENU,
	PLAY,
	INTRO,
	EDIT,
	PAUSE,
	CONTINUE,
};

class Game : public Align_16
{
private:
	gameState current = MENU;
	gameState last = MENU;
	Engine* engine = 0;
	ContentManager* content = 0;
	Player* player = 0;

	Map* map = 0;
	UserInput* in = 0;
	GUI* gui = 0;
	Edit* edit = 0;
	double lastX = 0.0f;
	double lastY = 0.0f;
	double xpos = 0.0f;
	double ypos = 0.0f;
	bool cameraFollow = false;
	mat4* viewMat = 0;

	GLFWwindow* windowRef;

	bool inBossRoom = false;
	glm::vec3 bossRoomMiddle;

	Progress playerProgress;
	Shrine* start;
	void checkForSave();
	void saveGame();

	glm::vec2 savedPickups[4];
	int savePickupNr = 0;
public:
	Game() {};
	~Game();
	void init(GLFWwindow* windowRef);
	void mainLoop();
	void update(float deltaTime);
	void readInput(float deltaTime);
	void buttonEvents(int buttonEv);
};

#endif