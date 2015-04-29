#include "Spark.h"

#define _USE_MATH_DEFINES
#include <math.h>

Spark::Spark()
{
	spawnX = 0;
	spawnY = 0;
	nrLights = 20;
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
		startDist[i + 1] = 2.0f*M_PI * d;
	}


	lights[0].init(spawnX, spawnY);
	lights[0].posZ = spawnZ;
	lights[0].r = 0.0f;
	lights[0].g = 1.0f;
	lights[0].b = 1.0f;
	lights[0].distance = 10;
	lights[0].intensity = 1;
	lights[0].volume = 1;

	timeLeft[0] = timeStart[0] = 1.0f;

	for (int i = 1; i < nrLights; i++)
	{
		float r = random(0.0f, 0.7f);
		lights[i].init(spawnX+cos(startDist[i]) * 2.0, spawnY+sin(startDist[i]) * 2.0);
		lights[i].posZ = 0;
		lights[i].r = 0.0;
		lights[i].g = 0.5;
		lights[i].b = 0.5;
		lights[i].distance = 2;
		lights[i].intensity = 1;
		lights[i].volume = 2;
		timeLeft[i] = timeStart[i] = r;
	}

}

void Spark::update()
{
	for (int i = 1; i < nrLights; i++)
	{
		if (timeLeft[i] < 0)
		{
			

			lights[i].init(spawnX + (cos(startDist[i]) * 0), spawnY + (sin(startDist[i]) * 0));
			lights[i].posZ = 0;
			lights[i].r = 0.0;
			lights[i].g = 0.5;
			lights[i].b = 0.5;
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
			lights[i].r = 0.0;
			lights[i].g = 0.5;
			lights[i].b = 0.5;
			lights[i].distance = 2 * (1 - t);
			lights[i].intensity = 1;
			lights[i].volume = 2;

		}

		timeLeft[i] -= 0.01f;
	}
}