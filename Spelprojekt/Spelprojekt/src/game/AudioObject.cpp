#include "AudioObject.h"


AudioObject::AudioObject()
{
}


AudioObject::~AudioObject()
{
	if (sourcePointer)
	{
		sourcePointer = NULL;
		delete sourcePointer;
	}
}

void AudioObject::init(bool is3D, int file, glm::vec3 pos, float dist, int loopType, int loopInterval)
{

	sourcePointer = NULL;
	looping = false;
	intervalLooping == false;
	interval = 0;
	time = 0;

	play3D = is3D;
	fileId = file;
	position = pos;
	distance = dist;
	if (loopType == 1)
		looping = true;
	else if (loopType == 2)
	{
		intervalLooping = true;
		interval = loopInterval;
	}
	state = A_WAITING;
	tracking = false;
	
}

void AudioObject::update(float deltaTime)
{
	if (intervalLooping == true) //plays sounds after an interval (does not support source pointer yet)
	{
		time += 10 * deltaTime;
		printf("Time: %f\n", time);

		if (time >= interval)
		{
			if (play3D) // 3D sound
				Audio::getAudio().playSoundAtPos(fileId, position, distance, looping);
			else // 2D sound
				Audio::getAudio().playSound(fileId, looping);

			time = 0;
		}
	}
	else if(intervalLooping == false) // play one sound and track it's position
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

		if (tracking)
		{
			if (objectPosition != NULL)
			{
				glm::vec3 tempPos = *objectPosition;
				ALfloat SourcePos[] = { tempPos.x, tempPos.y, tempPos.z };
				alSourcefv(*sourcePointer, AL_POSITION, SourcePos);
			}
		}
	}	
}

void AudioObject::bindToPosition(glm::vec3* pos, bool track)
{
	bool tracking = track;
	objectPosition = pos;
}
