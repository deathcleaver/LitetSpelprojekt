#include "Torch.h"

Torch::Torch()
{
	spawnX = 0;
	spawnY = 0;
	currentLights = nrLights = 40;
	lights = new Light[nrLights];
	timeLeft = new float[nrLights];
	timeStart = new float[nrLights];
	startDist = new float[nrLights];

	flameColor = new color[nrLights];

	for (int i = 0; i < nrLights; i++)
	{
		timeLeft[i] = timeStart[i] = 0.0;
		lights[i].distance = 0;
		lights[i].intensity = 0;
		startDist[i] = 0;
	}

	range = 0.5f;

	initSize = 5.0f;

	setBaseColor(1.0f, 1.0f, 1.0f);
	setParticleColor(0.8f, 0.8f, 0.1f);
	timeChangeColor(false, true, false);
}

Torch::~Torch()
{
	delete[] lights;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
	delete[] flameColor;
}

Light* Torch::getLights(int &nrLights)
{
	nrLights = currentLights;
	return lights;
}

void Torch::copy(BaseEffect* t)
{
	Torch* tOrg = (Torch*)t;

	spawnX = tOrg->spawnX;
	spawnY = tOrg->spawnY;
	spawnZ = tOrg->spawnZ;
	nrLights = tOrg->nrLights;
	currentLights = tOrg->currentLights;
	range = tOrg->range;

	setBaseColor(tOrg->baseR, tOrg->baseG, tOrg->baseB);
	setParticleColor(tOrg->particleR, tOrg->particleG, tOrg->particleB);

	for (int i = 0; i < nrLights; i++)
	{
		lights[i] = tOrg->lights[i];
		timeLeft[i] = tOrg->timeLeft[i];
		timeStart[i] = tOrg->timeStart[i];
		startDist[i] = tOrg->startDist[i];
		flameColor[i].r = tOrg->flameColor[i].r;
		flameColor[i].g = tOrg->flameColor[i].g;
		flameColor[i].b = tOrg->flameColor[i].b;
	}

}

void Torch::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;

	setBaseColor(1.0f, 1.0f, 1.0f);
	setParticleColor(0.8f, 0.8f, 0.1f);

	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = initSize;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i =1; i < nrLights; i++)
	{
		lights[i].init(spawnX, spawnY);
		lights[i].posZ = 0;
		lights[i].r = flameColor[i].r = particleR;
		lights[i].g = flameColor[i].g = particleG;
		lights[i].b = flameColor[i].b = particleB;
		lights[i].distance = startDist[i] = 0;
		lights[i].intensity = 1;
		lights[i].volume = 2;
		timeLeft[i] = timeStart[i] = 0;
	}

	
	timeChangeColor(false, true, false);

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
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = initSize;
	lights[0].intensity = 1;
	lights[0].volume = 1;
	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i = 1; i < currentLights; i++)
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
			lights[i].r = flameColor[i].r = particleR;
			lights[i].g = flameColor[i].g = particleG;
			lights[i].b = flameColor[i].b = particleB;
			lights[i].distance = startDist[i] = initSize;
			lights[i].intensity = 1;
			lights[i].volume = 2;
			timeLeft[i] = timeStart[i] = random(0.0f, 0.9f);
		}
		else
		{
			float t = (timeLeft[i] / timeStart[i]);
			lights[i].distance = startDist[i] * t;
			if (timeChangeR)
				lights[i].r = flameColor[i].r * t;
			if (timeChangeG)
				lights[i].g = flameColor[i].g * t;
			if (timeChangeB)
				lights[i].b = flameColor[i].b * t;
			lights[i].translate(0, 0.04f, 0);

		}
		timeLeft[i] -= 0.01f;
	}
}

void Torch::fade()
{
	for (int i = 0; i < currentLights; i++)
	{
		if (timeLeft[i] >= 0.0f)
		{
			float t = (timeLeft[i] / timeStart[i]);
			lights[i].distance = startDist[i] * t;
			if (i != 0)
			{
				if (timeChangeR)
					lights[i].r = particleR * t;
				if (timeChangeG)
					lights[i].g = particleG * t;
				if (timeChangeB)
					lights[i].b = particleB * t;
			}
			lights[i].translate(0, 0.04f, 0);

		}
		else
		{
			lights[i].distance = 0;
		}
		timeLeft[i] -= 0.01f;
	}
}

bool Torch::isFading()
{
	if (timeLeft[0] > 0)
		return true;
	return false;
}