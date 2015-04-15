#include "engine.h"

#include "Shader.h"

Engine::~Engine()
{

}

void Engine::init(glm::mat4* viewMat)
{
	gBuffer.init(800, 800, 3, true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearColor(0, 0, 0, 1);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);


	//temp camera
	viewMatrix = viewMat;
	projMatrix = glm::perspective(3.14f*0.45f, 800.f / 800.0f, 0.1f, 1000.0f);

	//Temp shader

	std::string shaders [] = {"src/shaders/default_vs.glsl", "src/shaders/gBuffer_fs.glsl" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	CreateProgram(tempshader, shaders, shaderType, 2);


	uniformModel = glGetUniformLocation(tempshader, "modelMatrix");
	uniformVP = glGetUniformLocation(tempshader, "VP");
}

void Engine::render(const Player* player, const EnemyManager* enemyManager,
	const Map* map, const ContentManager* content)
{
	// bind gbuffer FBO
	gBuffer.bind(GL_DRAW_FRAMEBUFFER);
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


	// bind default FBO and render gbuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	gBuffer.render();


}