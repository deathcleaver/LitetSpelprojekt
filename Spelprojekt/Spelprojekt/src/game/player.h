#ifndef PLAYER_H
#define PLAYER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <..\glm\glm.hpp>

class Player
{
private:
	glm::mat4 worldMat;
public:
	Player(){};
	void init();
	void bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
};

#endif