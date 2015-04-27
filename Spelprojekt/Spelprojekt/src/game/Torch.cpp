#include "Torch.h"

Torch::Torch()
{

	spawnX = 0;
	spawnY = 0;
	nrLights = 60;
	sparks = new Light[nrLights];
	timeLeft = new float[nrLights];
}

Torch::~Torch()
{
	delete[] sparks;
	delete[] timeLeft;
}

void Torch::init(float x, float y)
{
	spawnX = x;
	spawnY = y;

	sparks[0].init(spawnX, spawnY);
	sparks[0].posZ = 0;
	sparks[0].color(1, 1, 1);
	sparks[0].distance = 10;
	sparks[0].intensity = 1;
	sparks[0].volume = 1;

	for (int i = 1; i < nrLights; i++)
	{
		sparks[i].init(spawnX + 0.1*i, spawnY);
		sparks[i].posZ = 0;
		sparks[i].r = 1;
		sparks[i].g = 1;
		sparks[i].b = 1;
		sparks[i].distance = 1;
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
			sparks[i].init(spawnX + 0.1*i, spawnY);
			sparks[i].posZ = 0;
			sparks[i].r = nrLights / i;
			sparks[i].g = nrLights / i*i;
			sparks[i].b = 1/i;
			sparks[i].distance = 1;
			sparks[i].intensity = 1;
			sparks[i].volume = 2;
			timeLeft[i] = 1.0f;
		}
		else
		{
			sparks[i].r = nrLights % i;
			sparks[i].g = nrLights % (i*i);
			sparks[i].b = nrLights % i;
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