#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "game\game.h"

int main(int argv, char* argc[])
{
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

	while (!glfwWindowShouldClose(wnd))
	{
		glfwPollEvents();

		game.update(0.0f);
		glfwSwapBuffers(wnd);
	}

	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}