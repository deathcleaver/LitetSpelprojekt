#include "Lightning.h"

Lightning::Lightning()
{
	spawnX = 0;
	spawnY = 0;
	nrLights = 1;
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

	setBaseColor(1.0f, 1.0f, 1.0f);
	setParticleColor(0.8f, 0.8f, 0.1f);
	timeChangeColor(false, true, false);

	direction = false;
}

Lightning::~Lightning()
{
	delete[] lights;
	delete[] timeLeft;
	delete[] timeStart;
	delete[] startDist;
}

void Lightning::copy(BaseEffect* org)
{

}

void Lightning::init(float x, float y, float z)
{
	spawnX = x;
	spawnY = y;
	spawnZ = z;

	setBaseColor(1.0f, 1.0f, 1.0f);
	setParticleColor(0.8f, 0.8f, 0.1f);

	lights[0].init(spawnX, spawnY + 10);
	lights[0].posZ = spawnZ;
	lights[0].r = baseR;
	lights[0].g = baseG;
	lights[0].b = baseB;
	lights[0].distance = 3;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeStart[0] = 1.0f;

}

void Lightning::update()
{

	
	if (timeLeft[0] >= 0.0f)
	{
		float t = timeLeft[0] / timeStart[0];

		lights[0].init(spawnX, spawnY);
		lights[0].posZ = spawnZ;
		lights[0].r = baseR;
		lights[0].g = baseG;
		lights[0].b = baseB;
		lights[0].distance = 100000.0f * abs(sin(((float)M_PI / 2.0f) * t));
		lights[0].intensity = 1;
		lights[0].volume = 0;
	}
	else
	{
		lights[0].distance = 0;
	}

	if (direction)
	{
		timeLeft[0] -= 0.06f;
	}
	else
	{
		timeLeft[0] += 0.3f;
	}
	
	if (timeLeft[0] > timeStart[0])
	{
		direction = true;
	}
	if (timeLeft[0] < -10.0f)
	{
		direction = false;
	}

}

void Lightning::fade()
{

}

bool Lightning::isFading()
{
	return false;
}