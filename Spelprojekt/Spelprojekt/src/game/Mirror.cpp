#include "Mirror.h"

Mirror::Mirror()
{
	initialized = false;
	sizeX = 400;
	sizeY = 400;

	teleportLocation = glm::vec2(0, 0);
	chunkLocation = glm::vec2(0, 0);
	enterRect = 0;
}

Mirror::~Mirror()
{
	if (enterRect)
		delete enterRect;
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

	wMat = worldMat * glm::mat4(0.75f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.05f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f ,0.0f, 0.0f, 1.0f);
	
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;

	wMat[0].w = pos.x + 0.05f;
	wMat[1].w = pos.y + 0.03f;
	wMat[2].w = pos.z - 0.05f;

	p1 = wMat * p1;
	p2 = wMat * p2;
	p3 = wMat * p3;

	glm::vec4 d1 = p2 - p1;
	glm::vec4 d2 = p3 - p1;

	normal = glm::normalize(glm::cross(glm::vec3(d1.x, d1.y, d1.z), glm::vec3(d2.x, d2.y, d2.z)));
	normal.x = -normal.x;

	distance = 1;
	projMat = glm::perspective(3.14f * 0.45f, 1.0f / 1.5f, distance * 1.2f, 1000.0f);

}

void Mirror::initBoss()
{
	wMat[0].w += 0.5f;
	wMat[1].w += 0.25f;
	wMat[2].w -= 0.1f;
	distance = 8.5;
	projMat = glm::perspective(3.14f * 0.45f, 1.0f / 1.0f, distance * 1.2f, 1000.0f);
}

void Mirror::calcView()
{

	glm::vec3 pos;

	pos.x = wMat[0].w;
	pos.y = wMat[1].w;
	pos.z = wMat[2].w;

	viewMat = glm::lookAt(pos - (distance * normal), pos + (distance * normal), glm::vec3(0, 1, 0));
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

	mirrorBuffer.shaderMirrorPtr = screenBuffer.shaderMirrorPtr;

	mirrorBuffer.unifromNormal = screenBuffer.unifromNormal;
	mirrorBuffer.unifromWorld = screenBuffer.unifromWorld;

	mirrorBuffer.init(sizeX, sizeY, 4, true);
	initialized = true;


}

bool Mirror::isInitialized()
{
	return initialized;
}

