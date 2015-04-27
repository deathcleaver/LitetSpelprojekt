#ifndef TORCH
#define TORCH

#include <malloc.h>
#include "Light.h"

class Torch
{
	public:
	
		Torch();
		~Torch();
	
		void init(float x, float y);
		void update();
		Light* getLights(int &nrLights);
	
	private:
	
		Light* sparks;
		float* timeLeft;
	
		int nrLights;
	
		float spawnX, spawnY;

};

#endif