#include "GBuffer.h"

void Gbuffer::init(int x, int y, int nrTex, bool depth)
{
	if (renderQuad == 0)
	{
		genQuad();
	}

	glGenFramebuffers(1, &targetId);
	this->depth = depth;
	nrTextures = nrTex;

	generate(x, y);

	bind(GL_DRAW_FRAMEBUFFER);

	GLenum* DrawBuffers = new GLenum[nrTextures];
	
	for (size_t i = 0; i < nrTextures; i++)
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

	//gui
	uniformGUItexture = glGetUniformLocation(*shaderGuiPtr, "diffuse");
	//defaults 0
	glProgramUniform1i(*shaderGuiPtr, uniformGUItexture, 0);
	uniformGUIModel = glGetUniformLocation(*shaderGuiPtr, "modelMatrix");

	uniformBufferLightPos = glGetUniformBlockIndex(*shaderPtr, "lightBlock");
	uniformNrLightPos = glGetUniformLocation(*shaderPtr, "nrLights");
	
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
	

	// bind textures
	for (size_t i = 1; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		glProgramUniform1i(*shaderPtr, pos[i], i);
	}


	glBindBuffer(GL_ARRAY_BUFFER, lightBufferGlow);
	glGenVertexArrays(1, &LightVao);

	glBindVertexArray(LightVao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(Light), (void*)(sizeof(GLfloat)* 0));
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Light), (void*)(sizeof(GLfloat)* 4));
	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Light), (void*)(sizeof(GLfloat)* 8));

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(1, 1);

}

Gbuffer::~Gbuffer()
{
	delete[] rTexture;
	delete[] pos;
	//delete[] volume;
	glDeleteBuffers(1, &targetId);

	glDeleteBuffers(1, &lightBuffer);
	glDeleteBuffers(1, &lightBufferGlow);
}

void Gbuffer::resize(int x, int y)
{
	for (size_t i = 0; i < nrTextures; i++)
	{
		rTexture[i].resize(x, y);
	}
}

void Gbuffer::pushLights(Light* light, int lightsAdded)
{
	//if (nrLight + lightsAdded <= 100)
	//{
	//	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	//	glBufferSubData(GL_UNIFORM_BUFFER, nrLight * sizeof(Light), lightsAdded * sizeof(Light), light);
	//	for (int i = 0; i < lightsAdded; i++)
	//	{
	//		volume[i] = light[i].volume;
	//	}
	//	
	//	nrLight += lightsAdded;
	//}
	
	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lightBufferGlow);

	for (int  i = 0; i < lightsAdded && i < 1000; i++)
	{
		if (light[i].volume == 0 || light[i].volume == 1)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, nrLight * sizeof(Light), 1 * sizeof(Light), &light[i]);
			nrLight++;
		}
		if (light[i].volume == 2 || light[i].volume == 1)
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

void Gbuffer::render(glm::vec3* campos, const GUI* gui, const ContentManager* content, bool renderGui)
{
	
	// bind shader
	glUseProgram(*shaderPtr);

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	glProgramUniform3f(*shaderPtr, uniformCamPos, campos->x, campos->y, 1.0f);
	glProgramUniform3f(*shaderPtr, uniformPlayerPos, playerPos[0], playerPos[1], 1.0f);
	
	glProgramUniform1ui(*shaderPtr, uniformNrLightPos, nrLight);

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferLightPos, lightBuffer);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	renderGlow(campos);

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

void Gbuffer::generate(int x, int y)
{
	rTexture = new RenderTarget[nrTextures];
	for (size_t i = 0; i < nrTextures; i++)
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