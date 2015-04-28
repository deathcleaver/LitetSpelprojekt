#include "Torch.h"

Torch::Torch()
{
	spawnX = 0;
	spawnY = 0;
	nrLights = 80;
	sparks = new Light[nrLights];
	timeLeft = new float[nrLights];
	timeStart = new float[nrLights];
	startDist = new float[nrLights];
	range = 0.5f;
}

Torch::~Torch()
{
	delete[] sparks;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
}

void Torch::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;

	sparks[0].init(spawnX, spawnY);
	sparks[0].posZ = spawnZ;
	sparks[0].color(1, 1, 1);
	sparks[0].distance = 10;
	sparks[0].intensity = 1;
	sparks[0].volume = 1;

	for (int i = 1; i < nrLights; i++)
	{
		
		sparks[i].posZ = 0;
		sparks[i].r = 1;
		sparks[i].g = 1;
		sparks[i].b = 1;
		sparks[i].distance = startDist[i] = 1;
		sparks[i].intensity = 1;
		sparks[i].volume = 2;
		timeLeft[i] = 1.0/i;
	}

}

void Torch::update()
{
	for (int i = 1; i < nrLights; i++)
	{
		if (timeLeft[i] <= 0)
		{
			float spawnXrand = random(0, range);
			float spawnYrand = random(0, range);
			float spawnZrand = random(0, range);
			spawnXrand -= (range / 2);
			spawnYrand -= (range / 2);
			spawnZrand -= (range / 2);
			sparks[i].init(spawnX - spawnXrand, spawnY -spawnYrand );
			sparks[i].posZ = spawnZ - spawnZrand;
			sparks[i].r = 1.0f;
			sparks[i].g = 0.0f;
			sparks[i].b = 0.0f;
			sparks[i].distance = startDist[i] = 1;
			sparks[i].intensity = 1;
			sparks[i].volume = 2;
			timeLeft[i] = timeStart[i] = random(1.0f, 2.0f);
		}
		else
		{
			sparks[i].distance = startDist[i] * (timeLeft[i] / timeStart[i]);
			sparks[i].translate(0, 0.01, 0);

		}
		timeLeft[i] -= 0.01;
	}
}

Light* Torch::getLights(int &nrLights)
{
	nrLights = this->nrLights;
	return sparks;
}