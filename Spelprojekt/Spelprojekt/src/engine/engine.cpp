#include "engine.h"
#include "../GameConfig.h"

#include "Shader.h"

#include "../TimeQuery.h"

Engine::~Engine()
{
	delete eff;
	delete[]light;
	if (creditsBase)
		delete creditsBase;
	if (creditsObject)
		delete creditsObject;
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
	projMatrix = glm::perspective(3.14f*0.45f, (float)GameConfig::get().configResX / (float)GameConfig::get().configResY, 0.1f, 1000.0f);

	//Temp shader
	std::string shaders[] = { "src/shaders/default_vs.glsl", "src/shaders/gs.glsl", "src/shaders/default_fs.glsl" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	
	CreateProgram(tempshader, shaders, shaderType, 3);

	// mirror shader
	shaders[0] = "src/shaders/gBuffer_copy_vs.glsl";
	shaders[2] = "src/shaders/gBuffer_copy_fs.glsl";
	CreateProgram(mirrorShader, shaders, shaderType, 3);
	gBuffer.shaderMirrorPtr = &mirrorShader;

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

	// mirror
	mirrorModelMatrix  = glGetUniformLocation(mirrorShader, "modelMatrix");
	mirrorP = glGetUniformLocation(mirrorShader, "P");
	mirrorV = glGetUniformLocation(mirrorShader, "V");

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

	//gbuffer dof shader
	shaders[0] = "src/shaders/gBuffer_DoF_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_DoF_fs.glsl";
	CreateProgram(shaderDoF, shaders, shaderType, 2);
	gBuffer.shaderDoFPtr = &shaderDoF;

	gBuffer.init(GameConfig::get().configResX, GameConfig::get().configResY, 4, true);
	gBuffer.initDoF(GameConfig::get().configResX, GameConfig::get().configResY, 4, true);
	
	gBuffer.initLight();

	light = new Light[1000];

	fadeEffect = 0;

	fadeIn = false;
	fadeOut = false;

	eff = new Effect();
	eff->create(EffectType::lightning);
	eff->getEffect()->init(0, 0, 0);

	//credits object
	creditsObject = new Object("src/meshes/BaseBlit.v", "src/textures/credits.bmp");
	creditsBase = new GameObject();
	creditsBase->scaleAD(15.0f / 2, 130.0f / 2, 0.0f);
	creditsBase->moveTo(35 * 7, -5 * 35 + 17.5f, -1);

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
	const GUI* gui, glm::vec3* campos, int state, Edit* edit, UpdateAnimCheck* updateAnimCheck)
{

	int timerID = startTimer("Render");

	bossMirrorPass = true;
	renderMirrorPerspective(player, map, contentin, gui, campos, state, edit, updateAnimCheck);
	bossMirrorPass = false;

	// default FBO renderpass

	// bind gbuffer FBO
	gBuffer.clearLight();
	gBuffer.bind(GL_FRAMEBUFFER);
	glUseProgram(tempshader);
	glViewport(0, 0, GameConfig::get().configResX, GameConfig::get().configResY);
	// default renderpass

	glProgramUniformMatrix4fv(tempshader, uniformView, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(tempshader, uniformProj, 1, false, &projMatrix[0][0]);

	glProgramUniformMatrix4fv(tempshaderGBufferGlow, uniformViewGlow, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(tempshaderGBufferGlow, uniformProjGlow, 1, false, &projMatrix[0][0]);


	if (edit->forceRekts)
	{
		glProgramUniformMatrix4fv(tempshaderRekt, uniformRektView, 1, false, &(*viewMatrix)[0][0]);
		glProgramUniformMatrix4fv(tempshaderRekt, uniformRektProj, 1, false, &projMatrix[0][0]);
	}

	renderPass(player, map, contentin, gui, campos, state, edit, updateAnimCheck);

	bindLights(player, edit);


	// temp lightning
	int cx, cy;


	((Map*)map)->getChunkIndex(player->readPos(), &cx, &cy);

	if (cy == 0 && (cx == 2 || cx == 3 || cx == 4) && state == 1)
	{
		eff->update();
	}
	else
	{
		eff->getEffect()->fade();
	}

	int nr;
	Light* l = eff->getEffect()->getLights(nr);
	gBuffer.pushLights(l, nr);


	glProgramUniformMatrix4fv(mirrorShader, mirrorV, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(mirrorShader, mirrorP, 1, false, &projMatrix[0][0]);

	renderMirror();

	glDisable(GL_DEPTH_TEST);

	// renderPasses done, combine pass

	// bind default FBO and render gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glProgramUniform1f(tempshaderGBuffer, uniformFadePos, fadeEffect);
	glProgramUniform1f(tempshaderGBufferGlow, uniformFadePosGlow, fadeEffect);

	gBuffer.playerPos = &player->readPos()[0];

	if(state == 3 || state == 11)  // if state is Edit or Credits Put lights on cam
		gBuffer.render(campos, gui, map, content, true, edit->forceRekts, false);
	else // put lights on player
		gBuffer.render(campos, gui, map, content, true, edit->forceRekts, true);
    
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

	stopTimer(timerID);

	glUseProgram(0);
	

}


void Engine::renderMirrorPerspective(const Player* player, const Map* map, const ContentManager* contentin,
	const GUI* gui, glm::vec3* campos, int state, Edit* edit, UpdateAnimCheck* updateAnimCheck)
{

	width = map->readSizeX();
	height = map->readSizeY();
	chunks = map->getChunks();
	upDraw = map->getUpDraw();
	content = contentin;

	for (int n = 0; n < upDraw[0]; n++) // all chunks
	{
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
		if (upDraw[y] > -1 && upDraw[y] < height)
		{
			int size = chunks[upDraw[x]][upDraw[y]].gameObjects[WorldID::mirror].size(); //number of that ID this chunk has
			for (int k = 0; k < size; k++)
			{
				Mirror* obj = (Mirror*)chunks[upDraw[x]][upDraw[y]].gameObjects[WorldID::mirror][k];

				if (!obj->isInitialized())
				{
					obj->calculateNormal();
					if (upDraw[x] == 1 && upDraw[y] == 5)
						obj->initBoss();
					obj->calcView();

					obj->initGBuffer(gBuffer);
				}

				obj->mirrorBuffer.playerPos = (GLfloat*)&player->readPos();

				// mirror renderPass
				//obj->mirrorBuffer.bind(GL_FRAMEBUFFER);
				glBindFramebuffer(GL_FRAMEBUFFER, obj->targetId);
				glUseProgram(tempshader);
				glViewport(0, 0, 400, 400);

				glProgramUniformMatrix4fv(tempshader, uniformView, 1, false, &obj->viewMat[0][0]);
				glProgramUniformMatrix4fv(tempshader, uniformProj, 1, false, &obj->projMat[0][0]);

				glProgramUniformMatrix4fv(tempshaderGBufferGlow, uniformViewGlow, 1, false, &obj->viewMat[0][0]);
				glProgramUniformMatrix4fv(tempshaderGBufferGlow, uniformProjGlow, 1, false, &obj->projMat[0][0]);


				if (edit->forceRekts)
				{
					glProgramUniformMatrix4fv(tempshaderRekt, uniformRektView, 1, false, &obj->viewMat[0][0]);
					glProgramUniformMatrix4fv(tempshaderRekt, uniformRektProj, 1, false, &obj->projMat[0][0]);
				}

				renderPass(player, map, contentin, gui, campos, state, edit, updateAnimCheck);

			}
		}
	}
}

void Engine::renderMirror()
{
	for (int n = 0; n < upDraw[0]; n++) // all chunks
	{
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
		if (upDraw[y] > -1 && upDraw[y] < height)
		{
			int size = chunks[upDraw[x]][upDraw[y]].gameObjects[WorldID::mirror].size(); //number of that ID this chunk has
			for (int k = 0; k < size; k++)
			{
				Mirror* obj = (Mirror*)chunks[upDraw[x]][upDraw[y]].gameObjects[WorldID::mirror][k];
				//obj->bindWorldMat(&mirrorShader, &mirrorModelMatrix);
				glProgramUniformMatrix4fv(mirrorShader, mirrorModelMatrix, 1, false, &obj->wMat[0][0]);
				obj->render();
			}
		}
	}
}


void Engine::renderPass(const Player* player, const Map* map, const ContentManager* contentin,
	const GUI* gui, glm::vec3* campos, int state, Edit* edit, UpdateAnimCheck* updateAnimCheck)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 VP = projMatrix * *viewMatrix;
	

	int id = 0;
	int lastid = -1;
	width = map->readSizeX();
	height = map->readSizeY();
	chunks = map->getChunks();
	upDraw = map->getUpDraw();
	content = contentin;


	renderPlayer(player);

	if (state == 11 || state == 3) //if credits or editor
	{
		creditsObject->bind();
		creditsBase->bindWorldMat(&tempshader, &uniformModel);
		glDrawElementsInstanced(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_SHORT, 0, 1);
	}

	renderBack();


	renderWorld();

	renderMisc();

	renderEnemies(updateAnimCheck);

	renderEditObject(edit);
	

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
		if (WoID == WorldID::ghost_platform)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
			glColorMask(0, 0, 1, 1);
			glEnable(GL_CULL_FACE);
		}

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

		if (WoID == WorldID::ghost_platform)
		{
			glColorMask(1, 1, 1, 1);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
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
			{
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
				}

				HealthPickup* pickup = chunks[upDraw[x]][upDraw[y]].getPickup();
				if (pickup)
				{
					if (!pickup->isTaken())
					{
						id = pickup->bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bind(OBJ::MISC, MiscID::heart);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}
			}
	}
	lastid = -1;
}

void Engine::renderEnemies(UpdateAnimCheck* animCheck)
{
	//render chunk monsters
	for (int n = 0; n < upDraw[0]; n++){
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)
			{
				int size = chunks[upDraw[x]][upDraw[y]].countEnemies("Bat");
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::bat] = 1;
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
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::spikes] = 1;
				for (int i = 0; i < size; i++)
				{
					id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Spikes");
					if (id != lastid)
						facecount = content->bind(OBJ::ENEMY, EnemyID::spikes);
					glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
					lastid = id;
				}

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::flame] = 1;
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
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::cube] = 1;
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
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::ghost] = 1;
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

				glColorMask(1, 1, 1, 1);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_CULL_FACE);

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Spellbook");
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::spellbook] = 1;
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
				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Missile");
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::bat] = 1;
				for (int c = 0; c < size; c++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(c, "Missile"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(c, &tempshader, &uniformModel, "Missile");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::bat);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Spider");
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::spider] = 1;
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
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::web] = 1;
				for (int i = 0; i < size; i++)
				{
					id = chunks[upDraw[x]][upDraw[y]].bindEnemy(i, &tempshader, &uniformModel, "Web");
					if (id != lastid)
						facecount = content->bind(OBJ::ENEMY, EnemyID::web);
					glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
					lastid = id;
				}

				size = chunks[upDraw[x]][upDraw[y]].countEnemies("Webshot");
				if (size >= 1)
					animCheck->enemyUpdate[EnemyID::web] = 1;
				for (int c = 0; c < size; c++)
				{
					if (chunks[upDraw[x]][upDraw[y]].enemyLives(c, "Webshot"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(c, &tempshader, &uniformModel, "Webshot");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, EnemyID::web);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
					}
				}

				if (chunks[upDraw[x]][upDraw[y]].enemyLives(-1, "Boss"))
				{
					if (bossMirrorPass && chunks[upDraw[x]][upDraw[y]].enemyBlinking(-1, "Boss"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(-1, &tempshader, &uniformModel, "Boss");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
						animCheck->enemyUpdate[id] = 1;
					}
					else if (!chunks[upDraw[x]][upDraw[y]].enemyBlinking(-1, "Boss"))
					{
						id = chunks[upDraw[x]][upDraw[y]].bindEnemy(-1, &tempshader, &uniformModel, "Boss");
						if (id != lastid)
							facecount = content->bind(OBJ::ENEMY, id);
						glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
						lastid = id;
						animCheck->enemyUpdate[id] = 1;
						if (id >= EnemyID::grim_white && id <= EnemyID::grim_black)
						{
							for (int c = 0; c < 2; c++)
							{
								id = chunks[upDraw[x]][upDraw[y]].bindEnemy(c, &tempshader, &uniformModel, "GrimHand");
								if (id != lastid)
									facecount = content->bind(OBJ::ENEMY, id);
								glDrawElementsInstanced(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0, 1);
								lastid = id;
								animCheck->enemyUpdate[id] = 1;
							}
						}
					}
				}
			}

	}
	lastid = -1;
}

void Engine::bindLights(const Player* player, Edit* edit)
{
	// bind chunk lights
	
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
					int lightSize = 0;
					Light* chunkLights = chunks[upDraw[x]][upDraw[y]].getLights(lightSize);
					if (lightSize > 0)
					{
						gBuffer.forceGlow(chunkLights, lightSize);
					}

					if (chunks[upDraw[x]][upDraw[y]].shrine)
					{
						Light* temp = chunks[upDraw[x]][upDraw[y]].shrine->getLight();
						if (temp)
							gBuffer.forceGlow(temp, 1);
					}

					int flameCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
					lightSize = 0;
					for (int c = 0; c < flameCount; c++)
					{
						int nrLight = 0;
						Light* temp = chunks[upDraw[x]][upDraw[y]].getFlameLight(c, nrLight);
						gBuffer.forceGlow(temp, nrLight);
					}
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
					int lightSize = 0;
					Light* chunkLights = chunks[upDraw[x]][upDraw[y]].getLights(lightSize);
					if (lightSize > 0)
					{
						gBuffer.pushLights(chunkLights, lightSize);
					}

					if (chunks[upDraw[x]][upDraw[y]].shrine)
					{
						Light* temp = chunks[upDraw[x]][upDraw[y]].shrine->getLight();
						if (temp)
							gBuffer.pushLights(temp, 1);
					}

					int flameCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Flame");
					for (int c = 0; c < flameCount; c++)
					{
						int nrLight = 0;
						Light* temp = chunks[upDraw[x]][upDraw[y]].getFlameLight(c, nrLight);
						gBuffer.pushLights(temp, nrLight);
					}

					int ghostCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Ghost");
					for (int c = 0; c < ghostCount; c++)
					{
						int nrLight = 0;
						Light* temp = chunks[upDraw[x]][upDraw[y]].getGhostLight(c, nrLight);
						gBuffer.pushLights(temp, nrLight);
					}

					int missileCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Missile");
					for (int c = 0; c < missileCount; c++)
					{
						int nrLight = 0;
						Light* temp = chunks[upDraw[x]][upDraw[y]].getMissileLight(c, nrLight);
						gBuffer.pushLights(temp, nrLight);
					}

					int cubeCount = chunks[upDraw[x]][upDraw[y]].countEnemies("Cube");
					for (int c = 0; c < cubeCount; c++)
					{
						Light* temp = chunks[upDraw[x]][upDraw[y]].getCubeGlows(c);
						if (temp)
							gBuffer.pushLights(temp, 1);
					}

					HealthPickup* pickup = chunks[upDraw[x]][upDraw[y]].getPickup();
					if (pickup)
					{
						if (!pickup->isTaken())
						{
							Light* temp = pickup->getLight();
							if (temp)
								gBuffer.pushLights(temp, 1);
						}
					}
				}
		}
	}

		
	int nrLight = 0;
	Light* runeEffect = player->getRuneLight(nrLight);
	gBuffer.pushLights(runeEffect, nrLight);


	if (edit->getEditMode() == 2) //light editmode
	{
		Light* temp = edit->getLight();
		if (temp)
			gBuffer.pushLights(temp, 1);
	}

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

void Engine::setDoF(bool set)
{
	gBuffer.setDoF(set); 
}