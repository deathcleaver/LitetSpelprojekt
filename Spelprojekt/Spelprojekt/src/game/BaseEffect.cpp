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