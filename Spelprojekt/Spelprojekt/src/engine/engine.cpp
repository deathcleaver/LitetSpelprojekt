#include "engine.h"

#include "Shader.h"

Engine::~Engine()
{
	delete[]light;
}

void Engine::init(glm::mat4* viewMat)
{
	// init static variable

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 1);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);


	//temp camera
	viewMatrix = viewMat;
	projMatrix = glm::perspective(3.14f*0.45f, 1080 / 720.0f, 0.1f, 1000.0f);

	//Temp shader
	std::string shaders[] = { "src/shaders/default_vs.glsl", "src/shaders/gs.glsl", "src/shaders/default_fs.glsl" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	
	CreateProgram(tempshader, shaders, shaderType, 3);

	//gBuffer shader

	shaders[0] = "src/shaders/glow_vs.glsl";
	shaders[1] = "src/shaders/glow_gs.glsl";
	shaders[2] = "src/shaders/glow_fs.glsl";

	CreateProgram(tempshaderGBufferGlow, shaders, shaderType, 3);
	gBuffer.shaderGlowPtr = &tempshaderGBufferGlow;

	uniformProjGlow = glGetUniformLocation(tempshaderGBufferGlow, "P");
	uniformViewGlow = glGetUniformLocation(tempshaderGBufferGlow, "V");

	gBuffer.uniformCamPosGlow = glGetUniformLocation(tempshaderGBufferGlow, "camPos");

	shaders[0] = "src/shaders/gBuffer_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_fs.glsl";
	shaderType[1] = GL_FRAGMENT_SHADER;

	CreateProgram(tempshaderGBuffer, shaders, shaderType, 2);
	gBuffer.shaderPtr = &tempshaderGBuffer;

	uniformFadePos = glGetUniformLocation(tempshaderGBuffer, "fade");
	uniformFadePosGlow = glGetUniformLocation(tempshaderGBufferGlow, "fade");


	uniformModel = glGetUniformLocation(tempshader, "modelMatrix");
	uniformProj = glGetUniformLocation(tempshader, "P");
	uniformView = glGetUniformLocation(tempshader, "V");


	//gBuffer gui shader
	shaders[0] = "src/shaders/gBuffer_gui_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_gui_fs.glsl";
	CreateProgram(tempshaderGUI, shaders, shaderType, 2);
	gBuffer.shaderGuiPtr = &tempshaderGUI;

	
	gBuffer.init(1080, 720, 4, true);
	
	light = new Light[100];

	fadeEffect = 0;

	fadeIn = false;
	fadeOut = false;
}

void Engine::setFadeIn()
{
	fadeIn = true;
	fadeOut = false;
}

void Engine::setFadeOut()
{
	fadeIn = false;
	fadeOut = true;
}

void Engine::setFade(float fadeEffect)
{
	this->fadeEffect = fadeEffect;
}

void Engine::render(const Player* player, const Map* map, const ContentManager* content, 
	const GUI* gui, glm::vec3* campos, int state, Edit* edit)
{
	bool renderPlayer = false;
	bool renderBack = false;
	bool renderWorld = false;
	bool renderMonster = false;
	bool renderEditObject = false;
	bool renderGUI = true;
	bool renderRekts = false;
	bool renderGlow = false;


	gBuffer.playerPos = (GLfloat*)&player->readPos();
	
	switch (state)
	{
	case(0) : //MENU
		break;
	case(4) : //PAUSE
	case(1) : //PLAY
		renderPlayer = true;
		renderBack = true;
		renderWorld = true;
		renderMonster = true;
		renderGlow = true;
		break;
	case(2) : //INTRO
		
		break;
	case(3) : //EDIT
		renderBack = true;
		renderWorld = true;
		renderMonster = true;
		renderGlow = true;
		gBuffer.playerPos = (GLfloat*)campos;
		renderEditObject = true;
		if (edit->getEditMode() == EditMode::REKT)
			renderRekts = true;
		break;
	}

	int facecount = 0;

	// bind gbuffer FBO
	gBuffer.clearLight();
	gBuffer.bind(GL_FRAMEBUFFER);

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glUseProgram(tempshader);

	glm::mat4 VP = projMatrix * *viewMatrix;
	glProgramUniformMatrix4fv(tempshader, uniformView, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(tempshader, uniformProj, 1, false, &projMatrix[0][0]);

	glProgramUniformMatrix4fv(tempshaderGBufferGlow, uniformViewGlow, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(tempshaderGBufferGlow, uniformProjGlow, 1, false, &projMatrix[0][0]);

	// -- PlayerDraw --
	if (renderPlayer)
	{
		if (!player->isBlinking())
		{
			player->bindWorldMat(&tempshader, &uniformModel);
			facecount = content->bindPlayer();
			glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
		}
	}
	int id = 0;
	int lastid = -1;
	int width = map->readSizeX();
	int height = map->readSizeY();
	MapChunk** chunks = map->getChunks();
	int* upDraw = map->getUpDraw();

	//Render chunk background
	if(renderBack)
		for (int n = 0; n < upDraw[0]; n++){
			int x = n * 2 + 1;
			int y = x + 1;
			if (upDraw[x] > -1 && upDraw[x] < width)
				if (upDraw[y] > -1 && upDraw[y] < height)
					if (chunks[upDraw[x]][upDraw[y]].chunkBackground)
					{
						id = chunks[upDraw[x]][upDraw[y]].chunkBackground->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bindMapObj(id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
		}
	lastid = -1;

	//render chunk world objects
	
	if(renderWorld)
		for (int n = 0; n < upDraw[0]; n++){
			int x = n * 2 + 1;
			int y = x + 1;
			if (upDraw[x] > -1 && upDraw[x] < width)
				if (upDraw[y] > -1 && upDraw[y] < height)
				{
					//render boxes
					/*glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					glEnable(GL_CULL_FACE);
					glColorMask(0, 0, 1, 1);*/
					int size = chunks[upDraw[x]][upDraw[y]].Box_Objs.size();
					for (int k = 0; k < size; k++)
					{
						id = chunks[upDraw[x]][upDraw[y]].Box_Objs[k]->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bindMapObj(id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
					/*
					
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glDisable(GL_CULL_FACE);
					
					glColorMask(1, 1, 1, 1);*/

					//render mushrooms
					size = chunks[upDraw[x]][upDraw[y]].Mushroom_Objs.size();
					for (int k = 0; k < size; k++)
					{
						id = chunks[upDraw[x]][upDraw[y]].Mushroom_Objs[k]->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bindMapObj(id);
						glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
						lastid = id;
					}

					//render shrines
					if (chunks[upDraw[x]][upDraw[y]].shrine)
					{
						id = chunks[upDraw[x]][upDraw[y]].shrine->returnThis()->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bindMapObj(id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
						GameObject* rune = chunks[upDraw[x]][upDraw[y]].shrine->returnRune();
						if (rune)
						{
							id = rune->bindWorldMat(&tempshader, &uniformModel);
							if (id != lastid)
								facecount = content->bindMapObj(id);
							glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
							lastid = id;
						}
					}
				}
	}
	lastid = -1;

	//render chunk monsters
	if(renderMonster)
		for (int n = 0; n < upDraw[0]; n++){
			int x = n * 2 + 1;
			int y = x + 1;
			if (upDraw[x] > -1 && upDraw[x] < width)
				if (upDraw[y] > -1 && upDraw[y] < height)
				{
					int size = chunks[upDraw[x]][upDraw[y]].countEnemies("Bat");
					for (int i = 0; i < size; i++)
					{
						if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Bat") && !chunks[upDraw[x]][upDraw[y]].enemyBlinking(i, "Bat"))
						{
							id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Bat");
							if (id != lastid)
								facecount = content->bindMonsterObj(id);
							glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
							lastid = id;
						}
					}
					size = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
					for (int i = 0; i < size; i++)
					{
						if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Flame") && !chunks[upDraw[x]][upDraw[y]].enemyBlinking(i, "Flame"))
						{
							id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Flame");
							if (id != lastid)
								facecount = content->bindMonsterObj(id);
							glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
							lastid = id;
						}
					}
					size = chunks[upDraw[x]][upDraw[y]].countEnemies("Spikes");
					for (int i = 0; i < size; i++)
					{
						
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Spikes");
						if (id != lastid)
							facecount = content->bindMonsterObj(id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}

					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					glColorMask(0, 0, 1, 1);
					glEnable(GL_CULL_FACE);
					size = chunks[upDraw[x]][upDraw[y]].countEnemies("Cube");
					for (int i = 0; i < size; i++)
					{
						if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Cube") && !chunks[upDraw[x]][upDraw[y]].enemyBlinking(i, "Cube"))
						{
							id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Cube");
							if (id != lastid)
								facecount = content->bindMonsterObj(id);
							glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
							lastid = id;
						}
					}
					glColorMask(1, 1, 1, 1);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glDisable(GL_CULL_FACE);

					//Rendering the boss
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(-1, "Boss") && !chunks[upDraw[x]][upDraw[y]].enemyBlinking(-1, "Boss"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(-1, &tempshader, &uniformModel, "Boss");
						if (id != lastid)
							facecount = content->bindMonsterObj(id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}

				}

		}
	lastid = -1;
	
	//Render Edit Object
	if (renderEditObject)
	{
		GameObject* holder = edit->getObject();
		if (holder)
		{
			id = holder->bindWorldMat(&tempshader, &uniformModel);
			int editmode = edit->getEditMode();
			bool valid = false;
			switch (editmode)
			{
			case BACK:
				break;
			case WORLD:
				//check if valid
				if (id < content->nrOfWorldItems())
					facecount = content->bindMapObj(id);
				else
					edit->invalidID();
				break;
			case MONSTER:
				break;
			case REKT:
				break;
			case LIGHT:
				break;
			case NONEM:
				break;
			default:
				break;
			}
			glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
		}
	}


	 // bind chunk lights
	Light* chunkLights = 0;
	int lightSize = 0;
	int nrOfLights = 0;
	for (int n = 0; n < upDraw[0]; n++)
	{
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)
			{
				chunkLights = chunks[upDraw[x]][upDraw[y]].getLights(lightSize);
				if (lightSize > 0)
				{
					for (int c = 0; c < lightSize; c++)
					{
						light[nrOfLights + c].posX = chunkLights[c].posX;
						light[nrOfLights + c].posY = chunkLights[c].posY;
						light[nrOfLights + c].posZ = chunkLights[c].posZ;


						light[nrOfLights + c].r = chunkLights[c].r;
						light[nrOfLights + c].g = chunkLights[c].g;
						light[nrOfLights + c].b = chunkLights[c].b;

						light[nrOfLights + c].intensity = chunkLights[c].intensity;
						light[nrOfLights + c].distance = chunkLights[c].distance;
						light[nrOfLights + c].volume = chunkLights[c].volume;
					}
					nrOfLights += lightSize;
				}
				int flameCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
				lightSize = 0;
				for (int c = 0; c < flameCount; c++)
				{
					Light* temp = chunks[upDraw[x]][upDraw[y]].getFlameLight(c);
					if (temp)
					{
						
						light[nrOfLights + lightSize].posX = temp->posX;
						light[nrOfLights + lightSize].posY = temp->posY;
						light[nrOfLights + lightSize].posZ = temp->posZ;

						light[nrOfLights + lightSize].r = temp->r;
						light[nrOfLights + lightSize].g = temp->g;
						light[nrOfLights + lightSize].b = temp->b;

						light[nrOfLights + lightSize].intensity = temp->intensity;
						light[nrOfLights + lightSize].distance = temp->distance;
						light[nrOfLights + c].volume = temp->volume;
						lightSize++;
					}
				}
				nrOfLights += lightSize;
			}
		}
	Light* runeEffect = player->getRuneLight();
	if (runeEffect)
	{
		light[nrOfLights].posX = runeEffect->posX;
		light[nrOfLights].posY = runeEffect->posY;
		light[nrOfLights].posZ = runeEffect->posZ;

		light[nrOfLights].r = runeEffect->r;
		light[nrOfLights].g = runeEffect->g;
		light[nrOfLights].b = runeEffect->b;

		light[nrOfLights].intensity = runeEffect->intensity;
		light[nrOfLights].distance = runeEffect->distance;
		light[nrOfLights].volume = runeEffect->volume;
		nrOfLights++;
	}

	gBuffer.pushLights(light, nrOfLights);

	glDisable(GL_DEPTH_TEST);

	//if render collision rekts
	if (renderRekts)
	{
		glUseProgram(tempshader);
		glDisable(GL_DEPTH_TEST);
		content->bindRekt();
		GameObject temprekt;
		glm::vec3 pos;
		facecount = 2;

		for (int n = 0; n < upDraw[0]; n++)
		{
			int x = n * 2 + 1;
			int y = x + 1;
			if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)

			for (int xIndex = 0; xIndex < 35; xIndex++)
			for (int yIndex = 0; yIndex < 35; yIndex++)
			if (chunks[upDraw[x]][upDraw[y]].worldCollide[xIndex][yIndex] != NULL)
			{
				pos.x = (-17 + upDraw[x] * 35) + xIndex;
				pos.y = (17 - upDraw[y] * 35) - yIndex;
				pos.z = 0;
				temprekt.moveTo(pos);
				temprekt.bindWorldMat(&tempshader, &uniformModel);
				glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
			}
		}
	}

	// bind default FBO and render gbuffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glProgramUniform1f(tempshaderGBuffer, uniformFadePos, fadeEffect);
	glProgramUniform1f(tempshaderGBufferGlow, uniformFadePosGlow, fadeEffect);

	gBuffer.render(campos, gui, content, renderGUI);
    
	if (renderGlow)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		gBuffer.renderGlow(campos);
	}

	// fade
	


	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (fadeIn)
		fadeEffect += 0.01f;
	if (fadeOut)
		fadeEffect -= 0.01f;

	if (fadeEffect > 1.0)
		fadeIn = false;
	if (fadeEffect < 0.0)
		fadeOut = false;
}


