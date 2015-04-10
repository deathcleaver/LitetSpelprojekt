#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

class GameObject
{
private:
	float scale = 1.0f;
	glm::vec3 pos = { 0, 0, 0 };
	//rot
	glm::mat4 worldMat = { 1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 };
public:
	GameObject(){};

	void bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
	void rotate(float x, float y, float z);
	void translate(float x, float y, float z);
	void scaleUniform(float val);

};

#endif


