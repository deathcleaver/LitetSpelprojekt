#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "../Align16.h"

class GameObject : public Align_16
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
	void translate(float x, float y);
	void moveTo(float x, float y);
	void moveTo(float x, float y, float z);
	void moveTo(glm::vec3 target);

	void scaleUniform(float val);
	
	glm::vec3 readPos();

};

#endif


