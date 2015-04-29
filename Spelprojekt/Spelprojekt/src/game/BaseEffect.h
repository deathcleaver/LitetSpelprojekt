#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "Light.h"
#include <cstdlib>

#define random(min, max) ((max-min)*((float)rand()/RAND_MAX))+min;


class BaseEffect
{

public:

	virtual ~BaseEffect() {};

	virtual void init(float x, float y, float z);
	virtual void setSpawn(float x, float y, float z);

	virtual void copy(BaseEffect* t) = 0;

	virtual void update() = 0;

	virtual Light* getLights(int &nrLights);

protected:

	Light* lights;
	float* timeLeft;
	float* timeStart;
	float* startDist;

	int nrLights;

	float spawnX, spawnY, spawnZ;

	float range;

};

#endif