#include "AudioObject.h"


AudioObject::AudioObject()
{
}


AudioObject::~AudioObject()
{
	if (sourcePointer)
	{
		sourcePointer = NULL;
	}
}

void AudioObject::init(bool is3D, int file, glm::vec3 pos, float dist, int loopType, float minInt, float maxInt)
{

	sourcePointer = NULL;
	looping = false;
	intervalLooping == false;
	nextInstance = 0;
	time = 0;
	tracking = false;

	play3D = is3D;
	fileId = file;
	position = pos;
	distance = dist;
	if (loopType == 1)
		looping = true;
	else if (loopType == 2)
	{
		intervalLooping = true;
		minInterval = minInt;
		maxInterval = maxInt;
		nextInstance = minInt;
	}
	state = A_WAITING;
	
}

void AudioObject::update(float deltaTime)
{
	if (intervalLooping == true) //plays sounds after an interval (does not support source pointer yet)
	{
		time += 1 * deltaTime;
		//printf("Time: %f\n", time);

		if (time >= nextInstance)
		{
			if (play3D) // 3D sound
				Audio::getAudio().playSoundAtPos(fileId, position, distance, looping);
			else // 2D sound
				Audio::getAudio().playSound(fileId, looping);

			time = 0;
			float value = maxInterval - minInterval + 1;
			if (value < 1)
				value = 1;
			nextInstance = minInterval + (rand() % (int)(value));
			printf("Next source: %f\n", nextInstance);
		}
	}
	else// play one sound and track it's position
	{
		if (state == A_WAITING)
		{
			if (sourcePointer == NULL)
			{
				if (play3D) // 3D sound
					sourcePointer = Audio::getAudio().playSoundAtPosSP(fileId, position, distance, looping);
				else // 2D sound
					sourcePointer = Audio::getAudio().playSoundSP(fileId, looping);
			}

			state == A_PLAYING;
		}
	}	
}

void AudioObject::setPosition(glm::vec3 pos)
{
	if (sourcePointer != NULL)
	{
		ALfloat SourcePos[] = { pos.x, pos.y, pos.z };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcefv(sourcePointer, AL_POSITION, SourcePos);
		alSourcefv(sourcePointer, AL_VELOCITY, SourceVel);
	}
}

void AudioObject::setPitch(float pitch)
{
	if (sourcePointer != NULL)
	{
		alSourcef(sourcePointer, AL_PITCH, pitch);
	}
}

void AudioObject::stopSound()
{
	if (sourcePointer != NULL)
	{
		ALuint temp = sourcePointer;
			alSourceStop(temp);
	}
}

void AudioObject::getAllValues(int &type, int &id, float &posX, float &posY, float &posZ, float &dist, int &loopType, float &minInterval, float &maxInterval)
{
	type = play3D;
	id = fileId;
	posX = position.x; posY = position.y; posZ = position.z;
	dist = distance;
	if (looping == true)
		loopType = 1;
	else if (intervalLooping == true)
		loopType = 2;
	minInterval = this->minInterval; maxInterval = this->maxInterval;
}