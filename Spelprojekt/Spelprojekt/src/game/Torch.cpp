#include "Torch.h"

Torch::Torch()
{
	spawnX = 0;
	spawnY = 0;
	nrLights = 40;
	sparks = new Light[nrLights];
	timeLeft = new float[nrLights];
	timeStart = new float[nrLights];
	startDist = new float[nrLights];

	for (int i = 0; i < nrLights; i++)
	{
		timeLeft[i] = timeStart[i] = 0.0;
		sparks[i].distance = 0;
		sparks[i].intensity = 0;
		startDist[i] = 0;
	}

	range = 0.5f;
}

Torch::~Torch()
{
	delete[] sparks;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
}

void Torch::copy(Torch* t)
{
	spawnX = t->spawnX;
	spawnY = t->spawnY;
	spawnZ = t->spawnZ;
	nrLights = t->nrLights;
	range = t->range;

	for (int i = 0; i < nrLights; i++)
	{
		sparks[i] = t->sparks[i];
		timeLeft[i] = t->timeLeft[i];
		timeStart[i] = t->timeStart[i];
		startDist[i] = t->startDist[i];
	}

}

void Torch::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;

	sparks[0].init(spawnX, spawnY);
	sparks[0].posZ = spawnZ;
	sparks[0].color(1, 1, 1);
	sparks[0].distance = 7;
	sparks[0].intensity = 1;
	sparks[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i =1; i < nrLights; i++)
	{
		sparks[i].init(spawnX, spawnY);
		sparks[i].posZ = 0;
		sparks[i].r = 1;
		sparks[i].g = 1;
		sparks[i].b = 1;
		sparks[i].distance = startDist[i] = 10;
		sparks[i].intensity = 1;
		sparks[i].volume = 2;
		timeLeft[i] = timeStart[i] = random(0.0f, 0.7f);
	}

}

void Torch::setSpawn(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;
}

void Torch::update()
{
	sparks[0].init(spawnX, spawnY);
	sparks[0].posZ = spawnZ;
	sparks[0].color(1, 1, 1);
	sparks[0].distance = 7;
	sparks[0].intensity = 1;
	sparks[0].volume = 1;

	for (int i = 1; i < nrLights; i++)
	{
		if (timeLeft[i] < 0)
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
			sparks[i].g = 1.0f;
			sparks[i].b = 0.1f;
			sparks[i].distance = startDist[i] = 10;
			sparks[i].intensity = 1;
			sparks[i].volume = 2;
			timeLeft[i] = timeStart[i] = random(0.0f, 1.2f);
		}
		else
		{
			float t = (timeLeft[i] / timeStart[i]);
			sparks[i].distance = startDist[i] * t;
			sparks[i].r = 0.8f;
			sparks[i].g = 0.8 * t;
			sparks[i].b = 0.0f;
			sparks[i].translate(0, 0.04, 0);

		}
		timeLeft[i] -= 0.01;
	}
}

void Torch::fade()
{
	for (int i = 0; i < nrLights; i++)
	{
		if (timeLeft[i] >= 0.0f)
		{
			float t = (timeLeft[i] / timeStart[i]);
			sparks[i].distance = startDist[i] * t;
			if (i != 0)
			{
				sparks[i].r = 0.8f;
				sparks[i].g = 0.8 * t;
				sparks[i].b = 0.0f;
			}
			sparks[i].translate(0, 0.04, 0);

		}
		timeLeft[i] -= 0.01;
	}
}

bool Torch::isFading()
{
	if (timeLeft[0] > 0)
		return true;
	return false;
}

Light* Torch::getLights(int &nrLights)
{
	nrLights = this->nrLights;
	return sparks;
}