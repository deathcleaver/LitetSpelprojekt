#include "game.h"

Game::~Game()
{
	if (engine)
		delete engine;
}

void Game::init()
{
	engine = new Engine();
	engine->init();
	content = new ContentManager();
	content->init();
	player = new Player();
	player->init();


	//temp
	enemyman = new EnemyManager();
	map = new Map();
}

void Game::update(float deltatime)
{
	//Game code
	//..
	//..
	player->rotate(0, deltatime, 0);
	test += deltatime;
	player->translate(sin(test)/5, 0, 0);

	//Render const
	engine->render(player, enemyman, map, content);
}