#include "Mirror.h"

Mirror::Mirror()
{
	initialized = false;
	sizeX = 400;
	sizeY = 400;
}

Mirror::~Mirror()
{

}

void Mirror::calculateNormal()
{
	glm::vec4 p1 = glm::vec4(1, 1, 0, 1);
	glm::vec4 p2 = glm::vec4(1, -1, 0, 1);
	glm::vec4 p3 = glm::vec4(-1, 1, 0, 1);

	p1 = worldMat * p1;
	p2 = worldMat * p2;
	p3 = worldMat * p3;

	glm::vec4 d1 = p2 - p1;
	glm::vec4 d2 = p3 - p1;

	normal = glm::normalize(glm::cross(glm::vec3(d1.x, d1.y, d1.z), glm::vec3(d2.x, d2.y, d2.z)));
	normal.x = -normal.x;

}

void Mirror::calcView()
{

	glm::vec3 pos;

	pos.x = worldMat[0].w;
	pos.y = worldMat[1].w;
	pos.z = worldMat[2].w;

	viewMat = glm::lookAt(pos - normal, pos + normal, glm::vec3(0, 1, 0));
}

void Mirror::render()
{
	mirrorBuffer.renderTexture();
}

void Mirror::initGBuffer(Gbuffer &screenBuffer)
{

	mirrorBuffer.shaderDoFPtr = screenBuffer.shaderDoFPtr;
	mirrorBuffer.shaderGlowPtr = screenBuffer.shaderGlowPtr;
	mirrorBuffer.shaderGuiPtr = screenBuffer.shaderGuiPtr;
	mirrorBuffer.shaderPtr = screenBuffer.shaderPtr;
	mirrorBuffer.shaderRektPtr = screenBuffer.shaderRektPtr;
	mirrorBuffer.uniformCamPosGlow = screenBuffer.uniformCamPosGlow;

	mirrorBuffer.init(sizeX, sizeY, 4, true);
	initialized = true;

	//projMat = glm::ortho<float>(-0.8f, 0.8f, -1.0f, 1.0f, 0.01f, 1000.0f);
	projMat = glm::perspective(3.14f * 0.45f, 1.0f / 1.5f, 1.2f, 1000.0f);

}

bool Mirror::isInitialized()
{
	return initialized;
}

