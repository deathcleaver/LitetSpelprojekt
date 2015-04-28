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

#include "../Align16.h"

#include "GBuffer.h"


class Engine : public Align_16
{
private:
	// <temp>
	GLuint tempshader;
	GLuint tempshaderGBuffer;
	GLuint tempshaderGBufferGlow;
	GLuint tempshaderGUI;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProj;

	GLuint uniformViewGlow;
	GLuint uniformProjGlow;
	GLuint uniformFadePos;
	GLuint uniformFadePosGlow;


	glm::mat4* viewMatrix;
	glm::mat4 projMatrix;
	int cameraSwap = 0;
	int cameraSwapCounter = 0;
	// </temp>

	Gbuffer gBuffer;

	Light* light;

	float fadeEffect;
	bool fadeIn;
	bool fadeOut;

	void renderPlayer(const Player*);
	void renderBack();
	void renderWorld();
	void renderMisc();
	void renderEnemies();
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

public:
	Engine(){};
	~Engine();
	void init(glm::mat4* viewMatrix);
	void render(const Player* player, const Map* map, const ContentManager* content,
		const GUI* gui, vec3* campos, int state, Edit* edit);

	void setFadeIn();
	void setFadeOut();
	void setFade(float fadeEffect);
};

#endif