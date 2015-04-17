#ifndef GBUFFER_H
#define GBUFFER_H

#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "RenderTarget.h"

class Gbuffer : public RenderTarget
{
public:

	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void resize(int x, int y);

	void bind(GLuint index);

	void render(glm::vec3* campos);

	GLuint* shaderPtr;
	GLfloat* playerPos;
	GLfloat* cameraPos;

private:

	void generate(int x, int y);

	RenderTarget* rTexture;

	GLuint* pos;
	GLuint unifromCamPos;
	GLuint unifromPlayerPos;
};

#endif