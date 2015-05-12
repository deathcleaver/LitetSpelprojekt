#include "GBuffer.h"

Gbuffer::Gbuffer()
{
	targetId = 0;
	DoFBuffer = 0;
	lightBuffer = 0;
	lightBufferGlow = 0;

	initialized = false;
	lightInitialized = false;
	dofInitialize = false;
}

void Gbuffer::init(int x, int y, int nrTex, bool depth)
{
	renderDoF = false;

	if (renderQuad == 0)
	{
		genQuad();
	}

	// gbuffer

	glGenFramebuffers(1, &targetId);
	bind(GL_FRAMEBUFFER);
	this->depth = depth;
	nrTextures = nrTex;

	generate(x, y);

	

	GLenum* DrawBuffers = new GLenum[nrTextures];
	
	for (int i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = GL_NONE;
		}
		else
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = (GL_COLOR_ATTACHMENT0 + i);
		}
	}

	glDrawBuffers(nrTextures, DrawBuffers);

	delete[] DrawBuffers;

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	
	if (!shaderPtr)
	{
		throw;
	}

	pos = new GLuint[nrTextures];

	if (depth)
	{
		pos[0] = 0;
	}
	pos[1] = glGetUniformLocation(*shaderPtr, "diffuse");
	pos[2] = glGetUniformLocation(*shaderPtr, "normal");
	pos[3] = glGetUniformLocation(*shaderPtr, "world");

	uniformCamPos = glGetUniformLocation(*shaderPtr, "lightPos");
	uniformPlayerPos = glGetUniformLocation(*shaderPtr, "playerPos");

	playerPos = 0;

	//rekts
	uniformRektModel = glGetUniformLocation(*shaderRektPtr, "modelMatrix");

	//gui
	uniformGUItexture = glGetUniformLocation(*shaderGuiPtr, "diffuse");
	//defaults 0
	glProgramUniform1i(*shaderGuiPtr, uniformGUItexture, 0);
	uniformGUIModel = glGetUniformLocation(*shaderGuiPtr, "modelMatrix");
	
	uniformBufferLightPos = glGetUniformBlockIndex(*shaderPtr, "lightBlock");
	uniformNrLightPos = glGetUniformLocation(*shaderPtr, "nrLights");
	
	// mirror shader uniforms
	unifromNormal = glGetUniformLocation(*shaderMirrorPtr, "normalIn");
	unifromWorld = glGetUniformLocation(*shaderMirrorPtr, "worldIn");

	lightInitialized = false;
	dofInitialize = false;
	initialized = true;
}

void Gbuffer::initDoF(int x, int y, int nrTex, bool depth)
{
	if (targetId == 0)
		throw;
	// dof buffer
	glGenFramebuffers(1, &DoFBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, DoFBuffer);
	DoFTexture = new RenderTarget();
	DoFTexture->init(x, y, 0, false);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DoFTexture->getTargetId(), 0);

	GLenum drawBuffer = GL_COLOR_ATTACHMENT0;

	glDrawBuffers(1, &drawBuffer);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}

	// default
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//DoF shader uniforms
	uniformDoFBack = glGetUniformLocation(*shaderDoFPtr, "back");
	uniformDoFDepth = glGetUniformLocation(*shaderDoFPtr, "depth");

	dofInitialize = true;
}

void Gbuffer::initLight()
{
	if (targetId == 0)
		throw;
	glGenBuffers(1, &lightBuffer);
	glGenBuffers(1, &lightBufferGlow);

	nrLight = 1000;

	//volume = new int[nrLight];

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, nrLight * sizeof(Light), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBufferGlow);
	glBufferData(GL_UNIFORM_BUFFER, nrLight * sizeof(Light), NULL, GL_DYNAMIC_DRAW);

	nrLight = 0;
	nrLightGlow = 0;

	glBindBuffer(GL_ARRAY_BUFFER, lightBufferGlow);
	glGenVertexArrays(1, &LightVao);

	glBindVertexArray(LightVao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(Light), (void*)(sizeof(GLfloat) * 0));
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Light), (void*)(sizeof(GLfloat) * 4));
	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Light), (void*)(sizeof(GLfloat) * 8));

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(1, 1);

	// settings
	applySettings(true);

	lightInitialized = true;
}

void Gbuffer::applySettings(bool glows)
{
	glowsEnabled = glows;
}

Gbuffer::~Gbuffer()
{
	delete DoFTexture;
	delete[] rTexture;
	delete[] pos;
	//delete[] volume;
	if (initialized)
	{
		glDeleteFramebuffers(1, &targetId);
		Debug::DebugOutput("Deleting gbuffer target\n");
	}
	//

	if (dofInitialize)
	{
		glDeleteFramebuffers(1, &DoFBuffer);
		Debug::DebugOutput("Deleting gbuffer Dof buffer\n");
	}


	if (lightInitialized)
	{
		Debug::DebugOutput("Deleting gbuffer light buffers\n");
		glDeleteVertexArrays(1, &LightVao);
		//
		glDeleteBuffers(1, &lightBuffer);
		glDeleteBuffers(1, &lightBufferGlow);
	}

}

void Gbuffer::resize(int x, int y)
{
	for (int i = 0; i < nrTextures; i++)
	{
		rTexture[i].resize(x, y);
	}
}

void Gbuffer::pushLights(Light* light, int lightsAdded)
{

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lightBufferGlow);

	for (int  i = 0; i < lightsAdded && i < 1000; i++)
	{
		if ((light[i].volume == 0 || light[i].volume == 1) && nrLight < 1000)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, nrLight * sizeof(Light), 1 * sizeof(Light), &light[i]);
			nrLight++;
		}
		if ((light[i].volume == 2 || light[i].volume == 1) && nrLightGlow < 1000)
		{
			glBufferSubData(GL_ARRAY_BUFFER, nrLightGlow * sizeof(Light), 1 * sizeof(Light), &light[i]);
			nrLightGlow++;
		}
	}

}

void Gbuffer::forceGlow(Light* light, int lightsAdded)
{

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lightBufferGlow);

	for (int i = 0; i < lightsAdded && i < 1000; i++)
	{
		if ((light[i].volume == 0 || light[i].volume == 1) && nrLight < 1000)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, nrLight * sizeof(Light), 1 * sizeof(Light), &light[i]);
			nrLight++;
		}
		if (nrLightGlow < 1000)
		{
			glBufferSubData(GL_ARRAY_BUFFER, nrLightGlow * sizeof(Light), 1 * sizeof(Light), &light[i]);
			nrLightGlow++;
		}
	}

}


void Gbuffer::bind(GLuint target)
{
	glBindFramebuffer(target, targetId);
}

void Gbuffer::clearLight()
{
	nrLightGlow = 0;
	nrLight = 0;
}

void Gbuffer::renderGlow(glm::vec3* campos)
{
	// bind glowShader

	glUseProgram(*shaderGlowPtr);

	glProgramUniform3f(*shaderGlowPtr, uniformCamPosGlow, campos->x, campos->y, campos->z);

	glBindBuffer(GL_ARRAY_BUFFER, lightBufferGlow);
	glBindVertexArray(LightVao);

	//glDrawArrays(GL_POINTS, 0, nrLight);
	glDrawArraysInstanced(GL_POINTS, 0, 1, nrLightGlow);
	//for (int i = 0; i < nrLight; i++)
	//{
	//	glDrawArraysInstancedBaseInstance(GL_POINTS, 0, 1, min(volume[i], 1), i);
	//}

}

void Gbuffer::renderTexture()
{

	glUseProgram(*shaderMirrorPtr);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rTexture[1].getTargetId());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rTexture[2].getTargetId());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, rTexture[3].getTargetId());

	glProgramUniform1i(*shaderMirrorPtr, unifromNormal, 1);
	glProgramUniform1i(*shaderMirrorPtr, unifromWorld, 2);

	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


}

void Gbuffer::render(glm::vec3* campos, const GUI* gui, const Map* map, const ContentManager* content, bool renderGui, bool renderRektsEdit)
{

	// bind textures
	for (int i = 1; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		glProgramUniform1i(*shaderPtr, pos[i], i);
	}

	// bind shader
	glUseProgram(*shaderPtr);
	if (renderDoF)
		glBindFramebuffer(GL_FRAMEBUFFER, DoFBuffer);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	//campos
	glProgramUniform3f(*shaderPtr, uniformCamPos, campos->x, campos->y, 1.0f);
	
	//playerpos
	glProgramUniform3f(*shaderPtr, uniformPlayerPos, playerPos[0], playerPos[1], 1.0f);
	//glProgramUniform3f(*shaderPtr, uniformPlayerPos, playerPos[0], playerPos[1], 1.0f); //<--- DETTA MÅSTE FIXAS
	
	glProgramUniform1ui(*shaderPtr, uniformNrLightPos, nrLight);

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferLightPos, lightBuffer);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	//render depth of field colormap
	
	if (renderDoF)
	{
		//bind depth
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, rTexture[0].getTargetId());
		glProgramUniform1i(*shaderDoFPtr, uniformDoFDepth, 6);
		//bind back (diffuse at the momemnt)
		glActiveTexture(GL_TEXTURE0 + 7);
		glBindTexture(GL_TEXTURE_2D, DoFTexture->getTargetId());
		glProgramUniform1i(*shaderDoFPtr, uniformDoFBack, 7);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(*shaderDoFPtr);
		content->bindGUIvert(); //screen quad

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	if (glowsEnabled == true)
		renderGlow(campos);

	if (renderRektsEdit)
		renderRekts(map, content);


	if (renderGui)
	{
		glUseProgram(*shaderGuiPtr);

		int size = gui->readSize();
		ScreenItem** items = gui->getItems();
		//bind tex blit
		content->bindGUIvert();
		
		int id = 0;
		int lastid = -1;
		for (int n = 0; n < size; n++)
		{
			if (items[n]->visable())
			{
				id = items[n]->bindWorldMat(shaderGuiPtr, &uniformGUIModel);
				if (id != lastid)
					content->bindGUItex(id);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				lastid = id;
			}
		}
	}

}

void Gbuffer::renderRekts(const Map* map, const ContentManager* content)
{
	int* upDraw = map->getUpDraw();

	glUseProgram(*shaderRektPtr);
	glDisable(GL_DEPTH_TEST);
	content->bindRekt();
	GameObject temprekt;
	glm::vec3 pos;
	int width = map->readSizeX();
	int height = map->readSizeY();
	MapChunk** chunks = map->getChunks();


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
							pos.x = (float)((-17 + upDraw[x] * 35) + xIndex);
							pos.y = (float)((17 - upDraw[y] * 35) - yIndex);
							pos.z = 0;
							temprekt.moveTo(pos);
							temprekt.bindWorldMat(shaderRektPtr, &uniformRektModel);
							glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
						}
	}
}

void Gbuffer::generate(int x, int y)
{
	rTexture = new RenderTarget[nrTextures];
	for (int i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			rTexture[i].init(x, y, 0, true);
		}
		else
		{
			rTexture[i].init(x, y, 0, false);
		}
		
	}
}

void Gbuffer::setDoF(bool set)
{
	renderDoF = set;
}