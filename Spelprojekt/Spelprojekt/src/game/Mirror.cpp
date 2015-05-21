#include "Mirror.h"
#include "../engine/Shader.h"

GLuint Mirror::mirrorQuad = 0;
GLuint Mirror::mirrorVao = 0;

Mirror::Mirror()
{
	initialized = false;
	sizeX = 400;
	sizeY = 400;

	teleportLocation = glm::vec2(0, 0);
	chunkLocation = glm::vec2(0, 0);
	enterRect = 0;

	if (mirrorQuad == 0)
	{
		struct TriangleVertex
		{
			GLfloat x, y, z;
			GLfloat u, v;
		};

		TriangleVertex tri[4] =
		{
			0.8f, 1.05f, -0.05f,
			1.0f, 1.0f,

			0.8f, -1.05f, -0.05f,
			1.0f, 0.0f,

			-0.75f, 1.05f, -0.05f,
			0.0f, 1.0f,

			-0.75f, -1.05f, -0.05f,
			0.0f, 0.0f
		};

		glGenBuffers(1, &mirrorQuad);

		glBindBuffer(GL_ARRAY_BUFFER, mirrorQuad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

		glGenVertexArrays(1, &mirrorVao);
		glBindVertexArray(mirrorVao);

		// vertex in location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)0);

		// uv in location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)(sizeof(GLfloat) * 3));

	}

}

Mirror::~Mirror()
{
	if (enterRect)
		delete enterRect;

	glDeleteFramebuffers(1, &targetId);
	//glDeleteProgram(mirrorShader);

}

void Mirror::setRect()
{
	enterRect = new Rect();
	enterRect->initGameObjectRect(&worldMat, 2, 2);
}

void Mirror::calculateNormal()
{
	glm::vec4 p1 = glm::vec4(1, 1, 0, 1);
	glm::vec4 p2 = glm::vec4(-1, 1, 0, 1);
	glm::vec4 p3 = glm::vec4(1, -1, 0, 1);

	glm::vec3 pos;

	pos.x = worldMat[0].w;
	pos.y = worldMat[1].w;
	pos.z = worldMat[2].w;

	worldMat[0].w = 0;
	worldMat[1].w = 0;
	worldMat[2].w = 0;

	wMat = worldMat;// *glm::mat4(0.75f, 0.0f, 0.0f, 0.0f,
						//		0.0f, 1.05f, 0.0f, 0.0f,
						//		0.0f, 0.0f, 1.0f, 0.0f,
						//		0.0f ,0.0f, 0.0f, 1.0f);
	
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;

	wMat[0].w = pos.x;// + 0.05f;
	wMat[1].w = pos.y;// + 0.03f;
	wMat[2].w = pos.z;// - 0.05f;

	p1 = wMat * p1;
	p2 = wMat * p2;
	p3 = wMat * p3;

	glm::vec4 d1 = p2 - p1;
	glm::vec4 d2 = p3 - p1;

	normal = glm::normalize(glm::cross(glm::vec3(d1.x, d1.y, d1.z), glm::vec3(d2.x, d2.y, d2.z)));
	//normal.x = -normal.x;

	finalNormal = normal;

	distance = 1;
	projMat = glm::perspective(3.14f * 0.45f, 1.0f / 1.5f, distance * 1.2f, 1000.0f);

}

void Mirror::initBoss()
{
	//wMat[0].w += 0.5f;
	//wMat[1].w += 0.25f;
	//wMat[2].w -= 0.1f;
	distance = 8.5;

	rTexture[0].resize(800, 800);
	rTexture[1].resize(800, 800);
	rTexture[2].resize(800, 800);
	rTexture[3].resize(800, 800);

	projMat = glm::perspective(3.14f * 0.45f, 1.0f / 1.0f, distance * 1.2f, 1000.0f);

	if (enterRect)
		delete enterRect;
	enterRect = new Rect();
	enterRect->initGameObjectRect(&worldMat, 5, 5);
}

void Mirror::calcView()
{

	glm::vec3 pos;

	pos.x = wMat[0].w;
	pos.y = wMat[1].w;
	pos.z = wMat[2].w;

	viewMat = glm::lookAt(pos - (distance * finalNormal), pos + (distance * finalNormal), glm::vec3(0, 1, 0));
}

void Mirror::reflect(glm::vec3 camForward)
{
	glm::vec3 pos;

	pos.x = wMat[0].w;
	pos.y = wMat[1].w;
	pos.z = wMat[2].w;


	glm::vec3 d = glm::vec3(pos - camForward);

	finalNormal = glm::normalize(glm::reflect(d, normal));

}

void Mirror::render()
{
	//mirrorBuffer.renderTexture();
	glUseProgram(mirrorShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rTexture[1].getTargetId());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rTexture[2].getTargetId());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, rTexture[3].getTargetId());

	glProgramUniform1i(mirrorShader, unifromNormal, 1);
	glProgramUniform1i(mirrorShader, unifromWorld, 2);

	glBindBuffer(GL_ARRAY_BUFFER, mirrorQuad);
	glBindVertexArray(mirrorVao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Mirror::initGBuffer(Gbuffer &screenBuffer)
{

	mirrorBuffer.shaderDoFPtr = screenBuffer.shaderDoFPtr;
	mirrorBuffer.shaderGlowPtr = screenBuffer.shaderGlowPtr;
	mirrorBuffer.shaderGuiPtr = screenBuffer.shaderGuiPtr;
	mirrorBuffer.shaderPtr = screenBuffer.shaderPtr;
	mirrorBuffer.shaderRektPtr = screenBuffer.shaderRektPtr;
	mirrorBuffer.uniformCamPosGlow = screenBuffer.uniformCamPosGlow;

	mirrorBuffer.shaderMirrorPtr = screenBuffer.shaderMirrorPtr;

	mirrorBuffer.unifromNormal = screenBuffer.unifromNormal;
	mirrorBuffer.unifromWorld = screenBuffer.unifromWorld;

	//mirrorBuffer.init(sizeX, sizeY, 4, true);

	mirrorShader = *screenBuffer.shaderMirrorPtr;

	glGenFramebuffers(1, &targetId);
	
	glBindFramebuffer(GL_FRAMEBUFFER, targetId);

	int nrTextures = 4;
	bool depth = true;

	GLenum* DrawBuffers = new GLenum[nrTextures];

	for (int i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			rTexture[i].init(1024, 1024, 0, true);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = GL_NONE;
		}
		else
		{
			rTexture[i].init(1024, 1024, 0, false);
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

	
	std::string shaders[] = { "", "src/shaders/gs.glsl", "" };
	GLenum shaderType[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };

	// mirror shader
	shaders[0] = "src/shaders/gBuffer_copy_vs.glsl";
	shaders[2] = "src/shaders/gBuffer_copy_fs.glsl";
	
	//CreateProgram(mirrorShader, shaders, shaderType, 3);
	
	mirrorModelMatrix = glGetUniformLocation(mirrorShader, "modelMatrix");
	mirrorP = glGetUniformLocation(mirrorShader, "P");
	mirrorV = glGetUniformLocation(mirrorShader, "V");

	// mirror
	unifromNormal = glGetUniformLocation(mirrorShader, "normalIn");
	unifromWorld = glGetUniformLocation(mirrorShader, "worldIn");



	initialized = true;


}

bool Mirror::isInitialized()
{
	return initialized;
}

