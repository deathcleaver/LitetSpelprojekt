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

}

void AudioObject::update(float deltaTime)
{
	if (state == A_WAITING)
	{
		if (sourcePointer == NULL)
			sourcePointer = Audio::getAudio().playSoundAtPosSP(fileId, position, distance, looping);
		state == A_PLAYING;
	}
}
