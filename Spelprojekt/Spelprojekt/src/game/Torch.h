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
	
		void copy(Torch* t);

		void init(float x, float y, float z);
		void update();
		void fade();
		void setSpawn(float x, float y, float z);
		Light* getLights(int &nrLights);
	
		bool isFading();

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