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
	glClearColor(0, 0, 0, 1);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);


	//temp camera
	viewMatrix = viewMat;
	projMatrix = glm::perspective(3.14f*0.45f, 1080 / 720.0f, 0.1f, 1000.0f);

	//Temp shader

	std::string shaders [] = {"src/shaders/default_vs.glsl","src/shaders/gs.glsl" , "src/shaders/default_fs.glsl" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };

	CreateProgram(tempshader, shaders, shaderType, 3);

	shaders[0] = "src/shaders/gBuffer_vs.glsl";
	shaders[1] = "src/shaders/gBuffer_fs.glsl";
	shaderType[1] = GL_FRAGMENT_SHADER;
	
	CreateProgram(tempshaderGBuffer, shaders, shaderType, 2);

	gBuffer.shaderPtr = &tempshaderGBuffer;

	uniformModel = glGetUniformLocation(tempshader, "modelMatrix");
	uniformProj = glGetUniformLocation(tempshader, "P");
	uniformView = glGetUniformLocation(tempshader, "V");

	gBuffer.init(1080, 720, 4, true);

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
	glProgramUniformMatrix4fv(tempshader, uniformView, 1, false, &(*viewMatrix)[0][0]);
	glProgramUniformMatrix4fv(tempshader, uniformProj, 1, false, &projMatrix[0][0]);

	// -- PlayerDraw --
	player->bindWorldMat(&tempshader, &uniformModel);
	facecount = content->bindPlayer();
	glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);

	// - -Map Draw --
	int id = 0;
	int lastid = -1;
	int mapWidth = map->readSizeX();
	int mapHeight = map->readSizeY();
	int chunkCount = map->readSquareSize();
	MapChunk** chunks = map->getChunks();
	
	//backgrounds
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			if (chunks[x][y].chunkBackground)
			{
				id = chunks[x][y].chunkBackground->bindWorldMat(&tempshader, &uniformModel);
				if (id != lastid)
					facecount = content->bindMapObj(id);
				glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
				lastid = id;
			}
		}
	}
	lastid = -1;
	//world objects
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			for (int k = 0; k < chunks[x][y].countWorldObjs; k++)
			{
				id = chunks[x][y].worldObjs[k].bindWorldMat(&tempshader, &uniformModel);
				if (id != lastid)
					facecount = content->bindMapObj(id);
				glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
				lastid = id;
			}
		}
	}

	lastid = -1;
	//Enemies
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			for (int k = 0; k < chunks[x][y].countEnemies(); k++)
			{
				if (chunks[x][y].enemyLives(k))
				{
					id = chunks[x][y].bindEnemy(k, &tempshader, &uniformModel);
					if (id != lastid)
						facecount = content->bindMonsterObj(id);
					glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
					lastid = id;
				}
			}
		}
	}
    
    
	// bind default FBO and render gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
	gBuffer.render();
    
	glEnable(GL_DEPTH_TEST);

}


