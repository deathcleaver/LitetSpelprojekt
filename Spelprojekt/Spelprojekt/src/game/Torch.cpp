#include "Torch.h"

Torch::Torch()
{
	spawnX = 0;
	spawnY = 0;
	nrLights = 40;
	lights = new Light[nrLights];
	timeLeft = new float[nrLights];
	timeStart = new float[nrLights];
	startDist = new float[nrLights];

	for (int i = 0; i < nrLights; i++)
	{
		timeLeft[i] = timeStart[i] = 0.0;
		lights[i].distance = 0;
		lights[i].intensity = 0;
		startDist[i] = 0;
	}

	range = 0.5f;
}

Torch::~Torch()
{
	delete[] lights;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
}

void Torch::copy(BaseEffect* t)
{
	Torch* tOrg = (Torch*)t;

	spawnX = tOrg->spawnX;
	spawnY = tOrg->spawnY;
	spawnZ = tOrg->spawnZ;
	nrLights = tOrg->nrLights;
	range = tOrg->range;

	for (int i = 0; i < nrLights; i++)
	{
		lights[i] = tOrg->lights[i];
		timeLeft[i] = tOrg->timeLeft[i];
		timeStart[i] = tOrg->timeStart[i];
		startDist[i] = tOrg->startDist[i];
	}

}

void Torch::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;

	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].color(1, 1, 1);
	lights[0].distance = 7;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i =1; i < nrLights; i++)
	{
		lights[i].init(spawnX, spawnY);
		lights[i].posZ = 0;
		lights[i].r = 1;
		lights[i].g = 1;
		lights[i].b = 1;
		lights[i].distance = startDist[i] = 10;
		lights[i].intensity = 1;
		lights[i].volume = 2;
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
	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].color(1, 1, 1);
	lights[0].distance = 7;
	lights[0].intensity = 1;
	lights[0].volume = 1;

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
			lights[i].init(spawnX - spawnXrand, spawnY -spawnYrand );
			lights[i].posZ = spawnZ - spawnZrand;
			lights[i].r = 1.0f;
			lights[i].g = 1.0f;
			lights[i].b = 0.1f;
			lights[i].distance = startDist[i] = 7;
			lights[i].intensity = 1;
			lights[i].volume = 2;
			timeLeft[i] = timeStart[i] = random(0.0f, 0.9f);
		}
		else
		{
			float t = (timeLeft[i] / timeStart[i]);
			lights[i].distance = startDist[i] * t;
			lights[i].r = 0.8f;
			lights[i].g = 0.8 * t;
			lights[i].b = 0.0f;
			lights[i].translate(0, 0.04, 0);

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
			lights[i].distance = startDist[i] * t;
			if (i != 0)
			{
				lights[i].r = 0.8f;
				lights[i].g = 0.8 * t;
				lights[i].b = 0.0f;
			}
			lights[i].translate(0, 0.04, 0);

		}
		else
		{
			lights[i].distance = 0;
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
