#ifndef ENGINE_H
#define ENGINE_H


#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <..\glm\glm.hpp>
#include <..\glm\gtc\matrix_transform.hpp>

#include "../GUI.h"
#include "ContentManager.h"
#include "object.h"
#include "../game/player.h"
#include "../game/enemyManager.h"
#include "ObjectManager.h"
#include "../game/map.h"
#include "../game/edit.h"
#include "updateAnimCheck.h"

#include "../Align16.h"

#include "GBuffer.h"

#include "../game/Mirror.h"

class Engine : public Align_16
{
private:
	GLuint tempshader;
	GLuint tempshaderGBuffer;
	GLuint tempshaderGBufferGlow;
	GLuint tempshaderGUI;
	GLuint tempshaderRekt;
	GLuint shaderDoF;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProj;

	//glow
	GLuint uniformViewGlow;
	GLuint uniformProjGlow;
	GLuint uniformFadePos;
	GLuint uniformFadePosGlow;

	//rekt shader
	GLuint uniformRektView;
	GLuint uniformRektProj;

	//DoF shader
	GLuint sampleBack;
	GLuint sampleDepth;

	//Mirror copy shader
	GLuint mirrorShader;
	GLuint mirrorModelMatrix;
	GLuint mirrorV;
	GLuint mirrorP;

	glm::mat4* viewMatrix;
	glm::mat4 projMatrix;

	Gbuffer gBuffer;

	Light* light;

	float fadeEffect;
	bool fadeIn;
	bool fadeOut;

	void renderMirrorPerspective(const Player* player, const Map* map, const ContentManager* content,
		const GUI* gui, vec3* campos, int state, Edit* edit, UpdateAnimCheck* animCheck);

	void renderMirror();

	void renderPlayer(const Player*);
	void renderBack();
	void renderWorld();
	void renderMisc();
	void renderEnemies(UpdateAnimCheck* animCheck);
	void bindLights(const Player*, Edit* edit);
	void renderRekts();
	void renderEditObject(Edit*);

	int width = 0;
	int height = 0;
	MapChunk** chunks = 0;
	const ContentManager* content;
	int* upDraw = 0;
	int facecount;
	int id;
	int lastid;

	void renderPass(const Player* player, const Map* map, const ContentManager* content,
		const GUI* gui, vec3* campos, int state, Edit* edit, UpdateAnimCheck* animCheck);

public:
	void setDoF(bool set);
	
	Engine(){};
	~Engine();
	void init(glm::mat4* viewMatrix);
	void applySettings(bool glows);
	void render(const Player* player, const Map* map, const ContentManager* content,
		const GUI* gui, vec3* campos, int state, Edit* edit, UpdateAnimCheck* animCheck);

	void setFadeIn();
	void setFadeOut();
	void setFade(float fadeEffect);
};

#endif