#include "Spark.h"

Spark::Spark()
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
}

void Spark::update()
{

}