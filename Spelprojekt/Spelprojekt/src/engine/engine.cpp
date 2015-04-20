#include "engine.h"

#include "Shader.h"

Engine::~Engine()
{

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
	shaders[0] = "src/shaders/gBuffer_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_fs.glsl";
	shaderType[1] = GL_FRAGMENT_SHADER;

	CreateProgram(tempshaderGBuffer, shaders, shaderType, 2);
	gBuffer.shaderPtr = &tempshaderGBuffer;

	uniformModel = glGetUniformLocation(tempshader, "modelMatrix");
	uniformProj = glGetUniformLocation(tempshader, "P");
	uniformView = glGetUniformLocation(tempshader, "V");


	//gBuffer gui shader
	shaders[0] = "src/shaders/gBuffer_gui_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_gui_fs.glsl";
	CreateProgram(tempshaderGUI, shaders, shaderType, 2);
	gBuffer.shaderGuiPtr = &tempshaderGUI;

	
	gBuffer.init(1080, 720, 4, true);
}

void Engine::render(const Player* player, const Map* map, const ContentManager* content, const GUI* gui, glm::vec3* campos, int state)
{
	bool renderPlayer = false;
	bool renderBack = false;
	bool renderWorld = false;
	bool renderMonster = false;
	bool renderGUI = true;

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
		break;
	case(2) : //INTRO
		
		break;
	case(3) : //EDIT
		renderBack = true;
		renderWorld = true;
		renderMonster = true;
		gBuffer.playerPos = (GLfloat*)campos;
		break;
	}

	int facecount = 0;

	// bind gbuffer FBO
	gBuffer.bind(GL_FRAMEBUFFER);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glUseProgram(tempshader);

	glm::mat4 VP = projMatrix * *viewMatrix;
	glProgramUniformMatrix4fv(tempshader, uniformView, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(tempshader, uniformProj, 1, false, &projMatrix[0][0]);

	// -- PlayerDraw --
	if (renderPlayer)
	{
		player->bindWorldMat(&tempshader, &uniformModel);
		facecount = content->bindPlayer();
		if (!player->isBlinking())
		{
			glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
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
						glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
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
					for (int k = 0; k < chunks[upDraw[x]][upDraw[y]].countWorldObjs; k++)
					{
						id = chunks[upDraw[x]][upDraw[y]].worldObjs[k].bindWorldMat(&tempshader, &uniformModel);
						if (id != lastid)
							facecount = content->bindMapObj(id);
						glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
						lastid = id;
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
					for (int k = -1; k < chunks[upDraw[x]][upDraw[y]].countEnemies(); k++)
						if (chunks[upDraw[x]][upDraw[y]].enemyLives(k))
						{
							id = chunks[upDraw[x]][upDraw[y]].bindEnemy(k, &tempshader, &uniformModel);
							if (id != lastid)
								facecount = content->bindMonsterObj(id);
							glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
							lastid = id;
						}
		}
	lastid = -1;
				
	// bind default FBO and render gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
	gBuffer.render(campos, gui, content, renderGUI);
    
	glEnable(GL_DEPTH_TEST);

}


