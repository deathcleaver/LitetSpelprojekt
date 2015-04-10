#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"

#include "../engine/ContentManager.h"
#include "../engine/object.h"

#include "player.h"
#include "enemyManager.h"
#include "map.h"

#include <GLFW/glfw3.h>

class Game
{
private:
	Engine* engine = 0;
	ContentManager* content = 0;
	Player* player = 0;
	EnemyManager* enemyManager = 0;
	Map* map = 0;

	GLFWwindow* windowRef;
public:

	

	Game() {};
	~Game();
	void init(GLFWwindow* windowRef);
	void mainLoop();
	void update(float deltatime);
};

#endif