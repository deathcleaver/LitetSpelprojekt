#ifndef GBUFFER_H
#define GBUFFER_H

#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include "../GUI.h"
#include "RenderTarget.h"
#include "ContentManager.h"
#include "../game/Light.h"

class Gbuffer : public RenderTarget
{
public:

	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void resize(int x, int y);

	void bind(GLuint index);

	void render(glm::vec3* campos, const GUI* gui, const ContentManager* content, bool renderGui = true);

	GLuint* shaderPtr;
	GLuint* shaderGuiPtr;

	GLfloat* playerPos;

	Light* lights;
	int nrLight;

private:

	void generate(int x, int y);

	RenderTarget* rTexture;

	GLuint lightBuffer;

	GLuint* pos;
	GLuint uniformCamPos;
	GLuint uniformPlayerPos;

	GLuint uniformBufferLightPos;
	GLuint uniformNrLightPos;

	//gui
	GLuint uniformGUItexture;
	GLuint uniformGUIModel;

	

};

#endif