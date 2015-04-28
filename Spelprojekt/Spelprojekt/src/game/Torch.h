#ifndef TORCH
#define TORCH

#include <malloc.h>
#include "Light.h"
#include <cstdlib>

#define random(min, max) ((max - min) * ((((float) rand()) / (float) RAND_MAX)) + min );

class Torch
{
	public:
	
		Torch();
		~Torch();
	
		void init(float x, float y, float z);
		void update();
		Light* getLights(int &nrLights);
	
	private:
	
		Light* sparks;
		float* timeLeft;
		float* timeStart;
		float* startDist;
	
		int nrLights;
	
		float spawnX, spawnY, spawnZ;

		float range;

};

#endif