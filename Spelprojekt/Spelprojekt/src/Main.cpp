#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "game\game.h"

#include <ctime>

#include <vld.h>

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

	// init windowed or fullscreen
	bool fullscreen;
	ifstream in;
	char* settings = "Config/settings.s";
	in.open(settings);
	if (in)
	{
		string line;
		string sub;
		stringstream ss;
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		fullscreen = atoi(sub.c_str());

		in.close();
	}
	
	if (fullscreen) // fullscreen
		wnd = glfwCreateWindow(1080, 720, "ASUM PROJECT", glfwGetPrimaryMonitor(), NULL);
	else // windowed
		wnd = glfwCreateWindow(1080, 720, "ASUM PROJECT", NULL, NULL);

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