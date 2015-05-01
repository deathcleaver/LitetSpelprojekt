#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "game\game.h"

#include <ctime>

//#include <vld.h>

int main(int argv, char* argc[])
{
	srand(time(0));

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GLFWwindow* wnd;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, false);

	wnd = glfwCreateWindow(1080, 720, "ASUM PROJECT", NULL, NULL);
	//glfwCreateWindow(1920, 1080, "ASUM PROJECT", glfwGetPrimaryMonitor(), NULL);

	glfwMakeContextCurrent(wnd);
	glewInit();

	Game game;
	game.init(wnd);
	glfwSwapInterval(1);
	game.mainLoop();

	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}