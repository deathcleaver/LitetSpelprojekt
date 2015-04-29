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

void AudioObject::init(int file, glm::vec3 pos, float dist, float loop)
{
	sourcePointer = NULL;
	fileId = file;
	position = pos;
	distance = dist;
	looping = loop;
	state = A_WAITING;
	tracking = false;
	
}

void AudioObject::update(float deltaTime)
{
	if (state == A_WAITING)
	{
		if (sourcePointer == NULL)
			sourcePointer = Audio::getAudio().playSoundAtPosSP(fileId, position, distance, looping);
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

void AudioObject::bindToPosition(glm::vec3* pos, bool track)
{
	bool tracking = track;
	objectPosition = pos;
}
