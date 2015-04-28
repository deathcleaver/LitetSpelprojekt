#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include "Audio.h"

//states
#define A_PLAYING 0
#define A_WAITING 1

class AudioObject
{
public:
	AudioObject();
	~AudioObject();

	void init(int file, glm::vec3 pos, float dist, float loop);
	void update(float deltaTime);

private:


public:

private:
	ALuint* sourcePointer;
	int fileId, state;
	glm::vec3 position;
	float distance;
	float looping;

};

#endif