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
	void bindToPosition(glm::vec3* pos, bool track);

private:


public:

private:
	ALuint* sourcePointer;
	int fileId, state;
	glm::vec3 position;
	glm::vec3* objectPosition; //used for tracking and following a gameobject
	bool tracking;
	float distance;
	float looping;

};

#endif