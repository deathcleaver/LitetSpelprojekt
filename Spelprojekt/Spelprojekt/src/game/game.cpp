#include "game.h"
#include <ctime>

Game::~Game()
{
	if (engine)
		delete engine;
	if (content)
		delete content;
	if (player)
		delete player;
	if (enemyman)
		delete enemyman;
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
	enemyman = new EnemyManager();
	map = new Map();

	// do not delete in this class
	this->windowRef = windowRef;
}

void Game::mainLoop()
{
	clock_t start = clock();
	//how much of a second have passed since last frame
	float deltaTime = 0.01; //temp
	float clock;
	float lastClock = 0.0f;
	int fpsCount = 0;

	while (!glfwWindowShouldClose(windowRef))
	{
		glfwPollEvents();

		update(deltaTime);
		glfwSwapBuffers(windowRef);

		fpsCount++;
		clock = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		deltaTime = clock - lastClock;
		lastClock = clock;
		//Sleep(100);

		if (clock > 1)
		{
			start = std::clock();
			lastClock = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			std::string s = std::to_string(fpsCount);
			fpsCount = 0;
			glfwSetWindowTitle(windowRef, s.c_str());
		}
	}
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