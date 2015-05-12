#include "Lightning.h"
#include "../Audio.h"

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

	states[0] = 0.0f;
	states[1] = 1.0f;
	states[2] = 0.1f;
	states[3] = 0.7f;
	states[4] = 0.8f;
	states[5] = 0.65f;
	states[6] = 0.4f;
	states[7] = 0.76f;
	states[8] = 0.785f;
	states[9] = 0.1f;
	states[10] = 0.5f;


	flashState[0] = {0, 1, 0.5f };
	flashState[1] = { 1, 2, 1.5f };
	flashState[2] = { 2, 3, 0.3f };
	flashState[3] = { 3, 2, 0.7f };
	flashState[4] = { 2, 4, 0.2f };
	flashState[5] = { 4, 2, 0.7f };
	flashState[6] = { 2, 5, 0.25f };
	flashState[7] = { 5, 6, 0.45f };
	flashState[8] = { 6, 7, 0.1f };
	flashState[9] = { 7, 2, 0.6f };
	flashState[10] = { 2, 8, 0.3f };
	flashState[11] = { 8, 9, 0.6f };
	flashState[12] = { 9, 10, 0.35f };
	flashState[13] = { 10, 0, 3.0f };

	currState = 14;
	idleTime = -1.0f;
	idleStart = 5.0f;

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

	fade();

	if (idleTime < 0.0f)
	{
		idleTime = random(20.0f, 40.0f);
		currState = 0;
		idleStart = 5.0f;
	}

}

float Lightning::interpolate(float s0, float s1, float t)
{
	return (s0 * t) + (s1 * (1.0f - t));
}

void Lightning::fade()
{
	if (idleStart > 0.0f)
	{
		idleStart -= 0.1f;
	}
	else if (timeLeft[0] >= 0.0f && currState != 14)
	{
		float t = timeLeft[0] / timeStart[0];

		lights[0].init(spawnX, spawnY);
		lights[0].posZ = spawnZ;
		lights[0].r = baseR;
		lights[0].g = baseG;
		lights[0].b = baseB;
		lights[0].distance = 100000.0f * interpolate(states[flashState[currState].s1], states[flashState[currState].s2], t);
		lights[0].intensity = 3 * interpolate(states[flashState[currState].s1], states[flashState[currState].s2], t);
		lights[0].volume = 0;

		timeLeft[0] -= 0.1f;

		if (currState == 0 && timeLeft[0] <= 0.0f)
			Audio::getAudio().playSound(30, false); // lightning
		if (currState == 13 && timeLeft[13] <= 0.0f)
			Audio::getAudio().playSound(31, false); // thunder
	}
	else if (currState != 14)
	{
		//lights[0].distance = 0;
		currState++;

		float randState = random(0.0f, 13.0f);

		if (currState == 1)
		{
			timeLeft[0] = timeStart[0] = flashState[(int)randState].baseTime * random(1.4f, 1.9f);
		}
		else if (currState == 13)
		{
			timeLeft[0] = timeStart[0] = flashState[(int)randState].baseTime * random(2.0f, 3.0f);
		}
		else
		{
			timeLeft[0] = timeStart[0] = flashState[(int)randState].baseTime * random(0.3f, 0.8f);
		}

	}
	else
	{
		lights[0].distance = 0;
		idleTime -= 0.1f;
	}
}

bool Lightning::isFading()
{
	if (timeLeft[0] > 0)
	{
		return true;
	}
	else
	{
		return false;
		idleTime = -1.0f;
	}
}