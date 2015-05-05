#include "Shield.h"

Shield::Shield()
{
	spawnX = 0;
	spawnY = 0;
	spawnZ = 0;
	nrLights = 15;
	lights = new Light[nrLights];
	timeLeft = new float[nrLights];
	timeStart = new float[nrLights];
	startDist = new float[nrLights];

	for (int i = 0; i < nrLights - 1; i++)
	{
		float d = (float)i / (nrLights - 1);
		timeLeft[i + 1] = timeStart[i + 1] = 0.0;
		lights[i + 1].distance = 0;
		lights[i + 1].intensity = 0;
		startDist[i + 1] = 0;
	}

	setBaseColor(0, 1.0f, 0.0f);
	setParticleColor(0, 1.0f, 0.0f);

	range = 0.5f;
	rotation = 3.141596 / 2.0f;
}

Shield::~Shield()
{
	delete[] lights;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
}

void Shield::copy(BaseEffect* s)
{

}

void Shield::setRotation(float rot)
{
	rotation = rot;
}

void Shield::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z; 


	for (int i = 0; i < nrLights; i++)
	{
		timeLeft[i] = timeStart[0] = 0.0;
		lights[i].distance = 0;
		lights[i].intensity = 0;
		startDist[i] = 0;
	}


	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = 5;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i = 1; i < nrLights; i++)
	{
		float r = random(0.0f, 0.7f);
		float d = random(0.0f, 2.0f);
		startDist[i] = d - 1.0f;
		lights[i].init(spawnX, spawnY + (d - 1.0f));
		lights[i].posZ = 0;
		lights[i].r = particleR;
		lights[i].g = particleG;
		lights[i].b = particleB;
		lights[i].distance = 2;
		startDist[i] = d - 1.0f;
		lights[i].intensity = 1;
		lights[i].volume = 2;
		timeLeft[i] = timeStart[i] = r;
	}

}

void Shield::update()
{
	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = 5;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i = 1; i < nrLights; i++)
	{
		if (timeLeft[i] < 0)
		{
			float r = random(0.0f, 0.7f);
			float d = random(0.0f, 2.0f);
			startDist[i] = d - 1.0f;
			lights[i].init(spawnX, spawnY);
			lights[i].posZ = 0;
			lights[i].r = particleR;
			lights[i].g = particleG;
			lights[i].b = particleB;
			lights[i].distance = 2;
			lights[i].intensity = 1;
			lights[i].volume = 2;
			timeLeft[i] = timeStart[i] = r;
		}
		else
		{
			float t = (timeLeft[i] / timeStart[i]);
			lights[i].posZ = 0;
			lights[i].r = particleR;
			lights[i].g = particleG;
			lights[i].b = particleB;
			lights[i].distance = t;
			lights[i].intensity = 1;
			lights[i].volume = 2;
			if (startDist[i] < 0.0f)
				lights[i].translate(-0.02*cos(rotation), -0.02*sin(rotation), 0);
			else
				lights[i].translate(0.02*cos(rotation), 0.02*sin(rotation), 0);
		}

		timeLeft[i] -= 0.01f;
	}

}

void Shield::fade()
{
	for (int i = 0; i < nrLights; i++)
	{
		if (timeLeft[i] >= 0.0f)
		{
			float t = (timeLeft[i] / timeStart[i]);
			lights[i].posZ = 0;
			lights[i].r = particleR;
			lights[i].g = particleG;
			lights[i].b = particleB;
			lights[i].distance = t;
			lights[i].intensity = 1;
			lights[i].volume = 2;
			if (startDist[i] < 0.0f)
				lights[i].translate(-0.02*cos(rotation), -0.02*sin(rotation), 0);
			else
				lights[i].translate(0.02*cos(rotation), 0.02*sin(rotation), 0);
		}
		else
		{
			lights[i].distance = 0;
		}
		timeLeft[i] -= 0.01f;
	}
}

bool Shield::isFading()
{
	if (timeLeft[0] > 0)
		return true;
	return false;
}