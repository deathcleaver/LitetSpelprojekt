#include "BaseEffect.h"

void BaseEffect::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;
}

void BaseEffect::setSpawn(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;
}

Light* BaseEffect::getLights(int &nrLights)
{
	nrLights = this->nrLights;
	return lights;
}

void BaseEffect::setBaseColor(float r, float g, float b)
{
	baseR = r;
	baseG = g;
	baseB = b;
}

void BaseEffect::setParticleColor(float r, float g, float b)
{
	particleR = r;
	particleG = g;
	particleB = b;
}

void BaseEffect::timeChangeColor(bool r, bool g, bool b)
{
	timeChangeR = r;
	timeChangeG = g;
	timeChangeB = b;
}
