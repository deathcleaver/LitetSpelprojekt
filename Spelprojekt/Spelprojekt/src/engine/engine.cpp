#include "engine.h"

#include "Shader.h"

Engine::~Engine()
{

}

void Engine::init(glm::mat4* viewMat)
{
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

	std::string shaders [] = {"src/shaders/default_vs.glsl", "src/shaders/default_fs.glsl" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	CreateProgram(tempshader, shaders, shaderType, 2);


	uniformModel = glGetUniformLocation(tempshader, "modelMatrix");
	uniformVP = glGetUniformLocation(tempshader, "VP");
}

void Engine::render(const Player* player, const EnemyManager* enemyManager,
	const Map* map, const ContentManager* content)
{
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
	int chunkcount = map->readSquareSize();
	const MapChunk* chunks = map->getChunks();
	
	//backgrounds
	for (int n = 0; n < chunkcount; n++)
	{
		if(chunks[n].chunkBackground)
		{
			id = chunks[n].chunkBackground->bindWorldMat(&tempshader, &uniformModel);
			if(id != lastid)
				facecount = content->bindMapObj(id);
			glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
			lastid = id;
		}
	}
	lastid = -1;
	//world objects
	for (int n = 0; n < chunkcount; n++)
	{
		for (int k = 0; k < chunks[n].size; k++)
		{
			id = chunks[n].worldObjs[k].bindWorldMat(&tempshader, &uniformModel);
			if (id != lastid)
				facecount = content->bindMapObj(id);
			glDrawElements(GL_TRIANGLES, facecount * 3, GL_UNSIGNED_SHORT, 0);
			lastid = id;
		}
	}
}

void Engine::CompileErrorPrint(GLuint* shader)
{
	GLint success = 0;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success); //not working????
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(*shader, maxLength, &maxLength, &errorLog[0]);

		std::fstream myfile;
		myfile.open("errorCheck.txt", std::fstream::out);
		for (int i = 0; i < maxLength; i++)
		{
			myfile << errorLog[i];
		}
		myfile.close();

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(*shader); // Don't leak the shader.
		throw;
	}
}

void Engine::LinkErrorPrint(GLuint* shaderProgram)
{
	GLint success = 10;
	glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(*shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(*shaderProgram, maxLength, &maxLength, &errorLog[0]);

		std::fstream myfile;
		myfile.open("errorCheck.txt", std::fstream::out);
		for (int i = 0; i < maxLength; i++)
		{
			myfile << errorLog[i];
		}
		myfile.close();

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteProgram(*shaderProgram); // Don't leak the shader.

		if (success == GL_FALSE)
			throw;
	}
}