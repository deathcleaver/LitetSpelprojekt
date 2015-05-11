#ifndef GBUFFER_H
#define GBUFFER_H

#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>
#include "../GUI.h"
#include "RenderTarget.h"
#include "ContentManager.h"
#include "../game/Effect/Light.h"
#include "../game/map.h"
#include "ContentManager.h"

class Gbuffer : public RenderTarget
{
public:

	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void initLight();
	void initDoF(int x, int y, int nrTex, bool depth);
	void resize(int x, int y);

	void bind(GLuint index);

	void renderTexture();

	void render(glm::vec3* campos, const GUI* gui, const Map* map, const ContentManager* content, bool renderGui, bool renderRekts);

	void renderGlow(glm::vec3* campos);

	void renderRekts(const Map* map, const ContentManager* content);

	void clearLight();
	void pushLights(Light* light, int nrLight);
	void forceGlow(Light* light, int nrLight);

	void applySettings(bool glows);

	GLuint* shaderPtr;

	GLuint* shaderMirrorPtr;

	GLuint* shaderGlowPtr;
	GLuint* shaderGuiPtr;
	GLuint* shaderRektPtr;
	GLuint* shaderDoFPtr;

	// mirror
	GLuint unifromNormal;
	GLuint unifromWorld;

	GLfloat* playerPos;

	int nrLight;
	int nrLightGlow;

	//int* volume;

	GLuint uniformCamPosGlow;

	void setDoF(bool);
private:

	bool lightInitialized;
	bool dofInitialize;

	bool renderDoF;
	

	GLuint DoFBuffer;
	RenderTarget* DoFTexture;

	void generate(int x, int y);

	RenderTarget* rTexture;

	GLuint lightBuffer;
	GLuint lightBufferGlow;

	GLuint LightVao;


	GLuint* pos;
	GLuint uniformCamPos;
	
	GLuint uniformPlayerPos;
	GLuint uniformBufferLightPos;
	GLuint uniformNrLightPos;

	//gui
	GLuint uniformGUItexture;
	GLuint uniformGUIModel;

	//rekt
	GLuint uniformRektModel;

	//DoF
	GLuint uniformDoFBack;
	GLuint uniformDoFDepth;


	//settings
	bool glowsEnabled;
};

#endif