#include "engine.h"

#include "Shader.h"

Engine::~Engine()
{

}

void Engine::init(glm::mat4* viewMat)
{
	// init static variable
	//RenderTarget::renderQuad = 0;
	//RenderTarget::renderVao = 0;

	gBuffer.init(1080, 720, 3, true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearColor(0, 0, 0, 1);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);


	//temp camera
	viewMatrix = viewMat;
	projMatrix = glm::perspective(3.14f*0.45f, 1080 / 720.0f, 0.1f, 1000.0f);

	//Temp shader

	std::string shaders [] = {"src/shaders/default_vs.glsl", "src/shaders/gBuffer_fs.glsl" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	CreateProgram(tempshader, shaders, shaderType, 2);

	shaders[0] = "src/shaders/gBuffer_vs.glsl";
	shaders[1] = "src/shaders/default_fs.glsl";
	
	CreateProgram(tempshaderGBuffer, shaders, shaderType, 2);

	gBuffer.shaderPtr = &tempshaderGBuffer;

	uniformModel = glGetUniformLocation(tempshader, "modelMatrix");
	uniformVP = glGetUniformLocation(tempshader, "VP");
}

void Engine::render(const Player* player, const Map* map, const ContentManager* content)
{
	// bind gbuffer FBO
	gBuffer.bind(GL_FRAMEBUFFER);
	//


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int facecount = 0;
	glUseProgram(tempshader);
	
	glm::mat4 VP = projMatrix * *viewMatrix;
	glProgramUniformMatrix4fv(tempshader, uniformVP, 1, false, &VP[0][0]);

	// -- PlayerDraw --
	player->bindWorldMat(&tempshader, &uniformModel);
	facecount = content->bindPlayer();
	glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);

	// - -Map Draw --
	int id = 0;
	int lastid = -1;
	int width = map->readSizeX();
	int height = map->readSizeY();
	MapChunk** chunks = map->getChunks();
	int* upDraw = map->getUpDraw();
	
	//RenderChunks , X Y CULLED
	for (int n = 0; n < upDraw[0]; n++)
		if (upDraw[n * 2 + 1] > -1 && upDraw[n * 2 + 1] < width)
			if (upDraw[n * 2 + 2] > -1 && upDraw[n * 2 + 2] < height)
			{
				int x = n * 2 + 1;
				int y = n * 2 + 2;
				//Render chunk background
				if (chunks[upDraw[x]][upDraw[y]].chunkBackground)
				{
					id = chunks[upDraw[x]][upDraw[y]].chunkBackground->bindWorldMat(&tempshader, &uniformModel);
					if (id != lastid)
						facecount = content->bindMapObj(id);
					glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
					lastid = id;
				}
				lastid = -1;
				//render chunk world objects
				for (int k = 0; k < chunks[upDraw[x]][upDraw[y]].countWorldObjs; k++)
				{
					id = chunks[upDraw[x]][upDraw[y]].worldObjs[k].bindWorldMat(&tempshader, &uniformModel);
					if (id != lastid)
						facecount = content->bindMapObj(id);
					glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
					lastid = id;
				}
				lastid = -1;
				//render chunk monsters
				for (int k = 0; k < chunks[upDraw[x]][upDraw[y]].countEnemies(); k++)
				{
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
			}
    
	// bind default FBO and render gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
	gBuffer.render();
    
	glEnable(GL_DEPTH_TEST);

}


