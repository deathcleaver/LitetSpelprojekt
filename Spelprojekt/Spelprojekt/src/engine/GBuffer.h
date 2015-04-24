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

	void render(glm::vec3* campos, const GUI* gui, const ContentManager* content, bool renderGui);

	void renderGlow(glm::vec3* campos);

	void clearLight();
	void pushLights(Light* light, int nrLight);

	GLuint* shaderPtr;
	GLuint* shaderGlowPtr;
	GLuint* shaderGuiPtr;

	GLfloat* playerPos;

	int nrLight;

	int* volume;

	GLuint uniformCamPosGlow;

private:

	void generate(int x, int y);

	RenderTarget* rTexture;

	GLuint lightBuffer;

	GLuint LightVao;


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