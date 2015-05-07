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
}

bool Mirror::isInitialized()
{
	return initialized;
}

