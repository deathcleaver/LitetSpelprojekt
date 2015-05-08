#include "Spark.h"

Spark::Spark()
{
	spawnX = 0;
	spawnY = 0;
	currentLights = nrLights = 20;
	lights = new Light[nrLights];
	timeLeft = new float[nrLights];
	timeStart = new float[nrLights];
	startDist = new float[nrLights];

	for (int i = 0; i < nrLights-1; i++)
	{
		float d = (float)i / (nrLights - 1);
		timeLeft[i+1] = timeStart[i+1] = 0.0;
		lights[i+1].distance = 0;
		lights[i+1].intensity = 0;
		startDist[i+1] = 0;
	}

	setBaseColor(0, 1.0f, 1.0f);
	setParticleColor(0, 1.0f, 1.0f);

	range = 0.5f;
}

Spark::~Spark()
{
	delete[] lights;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
}

void Spark::copy(BaseEffect* s)
{
	Spark* sOrg = (Spark*)s;

	spawnX = sOrg->spawnX;
	spawnY = sOrg->spawnY;
	spawnZ = sOrg->spawnZ;
	nrLights = sOrg->nrLights;
	currentLights = sOrg->currentLights;
	range = sOrg->range;

	setBaseColor(sOrg->baseR, sOrg->baseG, sOrg->baseB);
	setParticleColor(sOrg->particleR, sOrg->particleG, sOrg->particleB);

	for (int i = 0; i < nrLights; i++)
	{
		lights[i] = sOrg->lights[i];
		timeLeft[i] = sOrg->timeLeft[i];
		timeStart[i] = sOrg->timeStart[i];
		startDist[i] = sOrg->startDist[i];
	}
}

void Spark::setSpawn(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;
}

void Spark::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;

	for (int i = 0; i < nrLights - 1; i++)
	{
		float d = (float)i / (nrLights - 1);
		timeLeft[i + 1] = timeStart[i + 1] = 0.0;
		lights[i + 1].distance = 0;
		lights[i + 1].intensity = 0;
		startDist[i + 1] = 2.0f*(float)M_PI * d;
	}


	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = 7;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i = 1; i < nrLights; i++)
	{
		float r = random(0.0f, 0.7f);
		lights[i].init(spawnX+cos(startDist[i]) * 0.0f, spawnY+sin(startDist[i]) * 0.0f);
		lights[i].posZ = 0;
		lights[i].r = particleR;
		lights[i].g = particleG;
		lights[i].b = particleB;
		lights[i].distance = 2;
		lights[i].intensity = 1;
		lights[i].volume = 2;
		timeLeft[i] = timeStart[i] = r;
	}

}

void Spark::update()
{
	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = 7;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i = 1; i < currentLights; i++)
	{
		if (timeLeft[i] < 0)
		{
			
			lights[i].init(spawnX + (cos(startDist[i]) * 0), spawnY + (sin(startDist[i]) * 0));
			lights[i].posZ = 0;
			lights[i].r = particleR;
			lights[i].g = particleG;
			lights[i].b = particleB;
			lights[i].distance = 2;
			lights[i].intensity = 1;
			lights[i].volume = 2;
			timeLeft[i] = timeStart[i] = random(0.0f, 0.7f);
		}
		else
		{
			float t = 1 - (timeLeft[i] / timeStart[i]);

			lights[i].init(spawnX + (cos(startDist[i]) * t), spawnY + (sin(startDist[i]) * t));
			lights[i].posZ = 0;
			lights[i].r = particleR;
			lights[i].g = particleG;
			lights[i].b = particleB;
			lights[i].distance = (1 - t);
			lights[i].intensity = 1;
			lights[i].volume = 2;

		}

		timeLeft[i] -= 0.01f;
	}
}

void Spark::fade()
{
	for (int i = 0; i < currentLights; i++)
	{
		if (timeLeft[i] >= 0.0f)
		{
			float t = 1 - (timeLeft[i] / timeStart[i]);

			lights[i].init(spawnX + (cos(startDist[i]) * t), spawnY + (sin(startDist[i]) * t));
			lights[i].posZ = 0;
			lights[i].r = particleR;
			lights[i].g = particleG;
			lights[i].b = particleB;
			lights[i].distance = (1 - t);
			lights[i].intensity = 1;
			lights[i].volume = 2;
		}
		else
		{
			lights[i].distance = 0;
		}

		timeLeft[i] -= 0.01f;
	}
}

bool Spark::isFading()
{
	if (timeLeft[0] > 0)
		return true;
	return false;
}