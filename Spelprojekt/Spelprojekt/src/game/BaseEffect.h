#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "Light.h"
#include <cstdlib>

#define random(min, max) ((max-min)*((float)rand()/RAND_MAX))+min;

#define _USE_MATH_DEFINES
#include <math.h>


class BaseEffect
{

public:

	virtual ~BaseEffect() {};

	virtual void init(float x, float y, float z);
	virtual void setSpawn(float x, float y, float z);
	virtual void setBaseColor(float r, float g, float b);
	virtual void setParticleColor(float r, float g, float b);

	virtual void timeChangeColor(bool r, bool g, bool b);

	virtual void copy(BaseEffect* t) = 0;

	virtual void update() = 0;

	virtual void fade();
	
	virtual bool isFading();

	virtual Light* getLights(int &nrLights);

protected:

	Light* lights;
	float* timeLeft;
	float* timeStart;
	float* startDist;

	int nrLights;

	float spawnX, spawnY, spawnZ;

	float baseR, baseG, baseB;
	float particleR, particleG, particleB;

	bool timeChangeR, timeChangeG, timeChangeB;

	float range;

};

#endif