#ifndef GAME_H
#define GAME_H

#include "../engine/engine.h"

#include "../engine/ContentManager.h"
#include "../engine/object.h"

#include "player.h"
#include "enemyManager.h"
#include "map.h"

class Game
{
private:
	Engine* engine = 0;
	ContentManager* content = 0;
	Player* player = 0;
	EnemyManager* enemyman = 0;
	Map* map = 0;

public:

	

	Game() {};
	~Game();
	void init();
	void update(float deltatime);
};

#endif