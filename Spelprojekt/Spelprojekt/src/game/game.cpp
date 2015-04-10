#include "game.h"
#include <ctime>
//#include <iostream>
//#include <Windows.h>

Game::~Game()
{
	if (engine)
		delete engine;
	if (content)
		delete content;
	if (player)
		delete player;
	if (enemyManager)
		delete enemyManager;
	if (map)
		delete map;
}

void Game::init(GLFWwindow* windowRef)
{
	engine = new Engine();
	engine->init();
	content = new ContentManager();
	content->init();
	player = new Player();
	player->init();

	//temp
	enemyManager = new EnemyManager();
	map = new Map();

	// do not delete in this class
	this->windowRef = windowRef;
}

void Game::mainLoop()
{
	clock_t start = clock();
	//how much of a second have passed since last frame
	float deltaTime = 0.0f;
	float clock;
	float lastClock = 0.0f;
	int fpsCount = 0;

	while (!glfwWindowShouldClose(windowRef))
	{
		glfwPollEvents();

		//std::cout << deltaTime << std::endl;
		//Sleep(250); deltatime test
		update(deltaTime);
		glfwSwapBuffers(windowRef);

		fpsCount++;
		clock = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		deltaTime = clock - lastClock;
		lastClock = clock;
		
		if (clock > 1)
		{
			start = std::clock();
			lastClock = 0;
			std::string s = std::to_string(fpsCount);
			fpsCount = 0;
			glfwSetWindowTitle(windowRef, s.c_str());
		}
	}
}

void Game::update(float deltaTime)
{
	//Game code
	//..
	//..
	player->update(deltaTime);

	//Render const
	engine->render(player, enemyManager, map, content);
}