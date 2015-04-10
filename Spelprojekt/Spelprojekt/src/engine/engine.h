#ifndef ENGINE_H
#define ENGINE_H


#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "ContentManager.h"
#include "object.h"
#include "../game/player.h"
#include "../game/enemyManager.h"
#include "../game/map.h"

class Engine
{
private:
	// <temp>
	GLuint tempshader;
	GLuint uniformModel;
	GLuint uniformVP;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	void Engine::CompileErrorPrint(GLuint* shader);
	void Engine::LinkErrorPrint(GLuint* shaderProgram);

	// </temp>

public:
	Engine(){};
	~Engine();
	void init();

	void render(const Player* player, const EnemyManager* enemyManager,
				const Map* map, const ContentManager* content);
};

#endif