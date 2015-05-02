#include "engine.h"

#include "Shader.h"

Engine::~Engine()
{
	delete e;
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

	//gbuffer Rekt Shader
	shaders[0] = "src/shaders/gBuffer_rekt_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_rekt_fs.glsl";
	CreateProgram(tempshaderRekt, shaders, shaderType, 2);
	gBuffer.shaderRektPtr = &tempshaderRekt;

	uniformRektProj = glGetUniformLocation(tempshaderRekt, "P");
	uniformRektView = glGetUniformLocation(tempshaderRekt, "V");

	gBuffer.init(1080, 720, 4, true);
	
	light = new Light[1000];

	fadeEffect = 0;

	fadeIn = false;
	fadeOut = false;

	e = new Effect();

	e->create(EffectType::shield);
	e->getEffect()->init(0, 2, 0);
}

void Engine::applySettings(bool glows)
{
	gBuffer.applySettings(glows);
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

void Engine::render(const Player* player, const Map* map, const ContentManager* contentin, 
	const GUI* gui, glm::vec3* campos, int state, Edit* edit)
{

	gBuffer.playerPos = (GLfloat*)&player->readPos();

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


	if (edit->forceRekts)
	{
		glProgramUniformMatrix4fv(tempshaderRekt, uniformRektView, 1, false, &(*viewMatrix)[0][0]);
		glProgramUniformMatrix4fv(tempshaderRekt, uniformRektProj, 1, false, &projMatrix[0][0]);
	}

	int id = 0;
	int lastid = -1;
	width = map->readSizeX();
	height = map->readSizeY();
	chunks = map->getChunks();
	upDraw = map->getUpDraw();
	content = contentin;

	renderPlayer(player);
	
	renderBack();

	renderWorld();

	renderMisc();

	renderEnemies();

	renderEditObject(edit);

	bindLights(player, edit);

	int nr = 0;
	Light* l;

	l = e->getEffect()->getLights(nr);

	e->getEffect()->update();

	gBuffer.pushLights(l, nr);

	glDisable(GL_DEPTH_TEST);

	// bind default FBO and render gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glProgramUniform1f(tempshaderGBuffer, uniformFadePos, fadeEffect);
	glProgramUniform1f(tempshaderGBufferGlow, uniformFadePosGlow, fadeEffect);

	gBuffer.render(campos, gui, map, content, true, edit->forceRekts);
    
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

void Engine::renderPlayer(const Player* player)
{
	// -- PlayerDraw --
	if (!player->isBlinking())
	{
		player->bindWorldMat(&tempshader, &uniformModel);
		facecount = content->bindPlayer();
		glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
	}
}

void Engine::renderBack()
{
	//Render chunk background
		for (int n = 0; n < upDraw[0]; n++)
		{
			int x = n * 2 + 1;
			int y = x + 1;
			if (upDraw[x] > -1 && upDraw[x] < width)
				if (upDraw[y] > -1 && upDraw[y] < height)
					if (chunks[upDraw[x]][upDraw[y]].chunkBackground)
					{
						id = chunks[upDraw[x]][upDraw[y]].chunkBackground->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bind(OBJ::BACK, id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
		}
		lastid = -1;
}

void Engine::renderWorld()
{
	//render chunk world objects
	for (int WoID = 0; WoID < WorldID::world_count; WoID++) // all IDS
	{
		for (int n = 0; n < upDraw[0]; n++) // all chunks
		{
			int x = n * 2 + 1;
			int y = x + 1;
			if (upDraw[x] > -1 && upDraw[x] < width)
				if (upDraw[y] > -1 && upDraw[y] < height)
				{
					int size = chunks[upDraw[x]][upDraw[y]].gameObjects[WoID].size(); //number of that ID this chunk has
					for (int k = 0; k < size; k++)
					{
						id = chunks[upDraw[x]][upDraw[y]].gameObjects[WoID][k]->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bind(WORLD, WoID);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}
		}
	}
	lastid = -1;
}

void Engine::renderMisc()
{
	for (int n = 0; n < upDraw[0]; n++)
	{
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)
				//render shrines
				if (chunks[upDraw[x]][upDraw[y]].shrine)
				{
					id = chunks[upDraw[x]][upDraw[y]].shrine->returnThis()->bindWorldMat(&tempshader, &uniformModel);
					if (id != lastid)
						facecount = content->bind(OBJ::MISC, MiscID::shrine);
					glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
					lastid = id;
					GameObject* rune = chunks[upDraw[x]][upDraw[y]].shrine->returnRune();
					if (rune)
					{
						id = rune->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bind(OBJ::MISC, id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}

					HealthPickup* pickup = chunks[upDraw[x]][upDraw[y]].getPickup();
					if (pickup)
					{
						if (!pickup->isTaken())
						{
							id = pickup->bindWorldMat(&tempshader, &uniformModel);
							if (id != lastid)
								facecount = content->bind(OBJ::MISC, id);
							glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
							lastid = id;
						}
					}
				}
	}
	lastid = -1;
}

void Engine::renderEnemies()
{
	//render chunk monsters
	for (int n = 0; n < upDraw[0]; n++){
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)
			{
				int size = chunks[upDraw[x]][upDraw[y]].countEnemies("Bat");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Bat"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Bat");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::bat);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}
				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Spikes");
				for (int i = 0; i < size; i++)
				{
					id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Spikes");
					if (id != lastid)
						facecount = content->bind(OBJ::ENEMY, EnemyID::spikes);
					glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
					lastid = id;
				}

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Flame"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Flame");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::flame);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}

				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glColorMask(0, 0, 1, 1);
				glEnable(GL_CULL_FACE);
				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Cube");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Cube"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Cube");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::cube);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Ghost");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Ghost") && !chunks[upDraw[x]][upDraw[y]].enemyBlinking(i, "Ghost"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Ghost");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::ghost);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}
				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Spellbook");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Spellbook"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Spellbook");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::spellbook);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}
				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Projectile");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Projectile"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Projectile");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::spellbook);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}

				glColorMask(1, 1, 1, 1);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_CULL_FACE);

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Spider");
				for (int i = 0; i < size; i++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(i, "Spider"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Spider");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::spider);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Web");
				for (int i = 0; i < size; i++)
				{
					id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Web");
					if (id != lastid)
						facecount = content->bind(OBJ::ENEMY, EnemyID::web);
					glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
					lastid = id;
				}

				if (chunks[upDraw[x]][upDraw[y]].enemyLives(-1, "Boss") && !chunks[upDraw[x]][upDraw[y]].enemyBlinking(-1, "Boss"))
				{
					id = chunks[upDraw[x]][upDraw[y]].bindEnemy(-1, &tempshader, &uniformModel, "Boss");
					if (id != lastid)
						facecount = content->bind(OBJ::ENEMY, EnemyID::bat);
					glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
					lastid = id;
				}
			}

	}
	lastid = -1;
}

void Engine::bindLights(const Player* player, Edit* edit)
{
	// bind chunk lights
	Light* chunkLights = 0;
	int lightSize = 0;
	int nrOfLights = 0;
	
	//If moving lights in the editor, force glow
	if (edit->isMovingLights())
	{
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
							light[nrOfLights + c].volume = 1;
						}
						nrOfLights += lightSize;
					}

					if (chunks[upDraw[x]][upDraw[y]].shrine)
					{
						Light* temp = chunks[upDraw[x]][upDraw[y]].shrine->getLight();
						light[nrOfLights].posX = temp->posX;
						light[nrOfLights].posY = temp->posY;
						light[nrOfLights].posZ = temp->posZ;

						light[nrOfLights].r = temp->r;
						light[nrOfLights].g = temp->g;
						light[nrOfLights].b = temp->b;

						light[nrOfLights].intensity = temp->intensity;
						light[nrOfLights].distance = temp->distance;
						light[nrOfLights].volume = 0;
					}

					int flameCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
					lightSize = 0;
					for (int c = 0; c < flameCount; c++)
					{
						int nrLight = 0;
						Light* temp = chunks[upDraw[x]][upDraw[y]].getFlameLight(c, nrLight);
						for (int i = 0; i < nrLight; i++)
						{
							light[nrOfLights + lightSize].posX = temp[i].posX;
							light[nrOfLights + lightSize].posY = temp[i].posY;
							light[nrOfLights + lightSize].posZ = temp[i].posZ;

							light[nrOfLights + lightSize].r = temp[i].r;
							light[nrOfLights + lightSize].g = temp[i].g;
							light[nrOfLights + lightSize].b = temp[i].b;

							light[nrOfLights + lightSize].intensity = temp[i].intensity;
							light[nrOfLights + lightSize].distance = temp[i].distance;
							light[nrOfLights + lightSize].volume = 0;
							lightSize++;
						}
					}
					nrOfLights += lightSize;
				}
		}
	}
	else // if editor is not moving lights
	{
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

					if (chunks[upDraw[x]][upDraw[y]].shrine)
					{
						Light* temp = chunks[upDraw[x]][upDraw[y]].shrine->getLight();
						light[nrOfLights].posX = temp->posX;
						light[nrOfLights].posY = temp->posY;
						light[nrOfLights].posZ = temp->posZ;

						light[nrOfLights].r = temp->r;
						light[nrOfLights].g = temp->g;
						light[nrOfLights].b = temp->b;

						light[nrOfLights].intensity = temp->intensity;
						light[nrOfLights].distance = temp->distance;
						light[nrOfLights].volume = temp->volume;
						nrOfLights++;
					}

					int flameCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
					lightSize = 0;
					for (int c = 0; c < flameCount; c++)
					{
						int nrLight = 0;
						Light* temp = chunks[upDraw[x]][upDraw[y]].getFlameLight(c, nrLight);
						for (int i = 0; i < nrLight; i++)
						{
							light[nrOfLights + lightSize].posX = temp[i].posX;
							light[nrOfLights + lightSize].posY = temp[i].posY;
							light[nrOfLights + lightSize].posZ = temp[i].posZ;

							light[nrOfLights + lightSize].r = temp[i].r;
							light[nrOfLights + lightSize].g = temp[i].g;
							light[nrOfLights + lightSize].b = temp[i].b;

							light[nrOfLights + lightSize].intensity = temp[i].intensity;
							light[nrOfLights + lightSize].distance = temp[i].distance;
							light[nrOfLights + lightSize].volume = temp[i].volume;
							lightSize++;
						}
					}
					nrOfLights += lightSize;

					HealthPickup* pickup = chunks[upDraw[x]][upDraw[y]].getPickup();
					if (pickup)
					{
						if (!pickup->isTaken())
						{
							Light* temp = pickup->getLight();
							light[nrOfLights].posX = temp->posX;
							light[nrOfLights].posY = temp->posY;
							light[nrOfLights].posZ = temp->posZ;

							light[nrOfLights].r = temp->r;
							light[nrOfLights].g = temp->g;
							light[nrOfLights].b = temp->b;

							light[nrOfLights].intensity = temp->intensity;
							light[nrOfLights].distance = temp->distance;
							light[nrOfLights].volume = temp->volume;
							nrOfLights++;
						}
					}
				}
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

	if (edit->getEditMode() == 2) //light editmode
	{
		Light* temp = edit->getLight();
		if (temp)
		{
			light[nrOfLights].posX = temp->posX;
			light[nrOfLights].posY = temp->posY;
			light[nrOfLights].posZ = temp->posZ;

			light[nrOfLights].r = temp->r;
			light[nrOfLights].g = temp->g;
			light[nrOfLights].b = temp->b;

			light[nrOfLights].intensity = temp->intensity;
			light[nrOfLights].distance = temp->distance;
			light[nrOfLights].volume = temp->volume;
			nrOfLights++;
		}
	}

	gBuffer.pushLights(light, nrOfLights);
}

void Engine::renderEditObject(Edit* edit)
{
	//Render Edit Object
	GameObject* holder = edit->getObject();
	if (holder)
	{
		id = holder->bindWorldMat(&tempshader, &uniformModel);
		int editmode = edit->getEditMode();
		bool valid = false;
		switch (editmode)
		{
		case 0: // World Mode
			//check if valid
			if (id < content->nrOfWorldItems())
			{
				facecount = content->bind(OBJ::WORLD, id);
				valid = true;
			}
			else
				edit->invalidID();
			break;
		}

		if (valid)
			glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
	}
}
