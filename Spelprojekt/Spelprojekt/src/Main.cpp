#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <windows.h>
#include <ctime>
#include "game\game.h"

int main(int argv, char* argc[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GLFWwindow* wnd;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	wnd = glfwCreateWindow(800, 800, "ASUM PROJECt", NULL, NULL);

	glfwMakeContextCurrent(wnd);
	glewInit();

	Game game;
	game.init();

	clock_t start = clock();
	//how much of a second have passed since last frame
	float deltaTime = 0.01; //temp
	float clock;
	float lastClock = 0.0f;
	int fpsCount = 0;

	while (!glfwWindowShouldClose(wnd))
	{
		glfwPollEvents();
		
		game.update(deltaTime);
		glfwSwapBuffers(wnd);

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
			std::wstring stemp = std::wstring(s.begin(), s.end());
			LPCWSTR sw = stemp.c_str();
			//SetWindowText(wnd, sw);
		}
	}

	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}