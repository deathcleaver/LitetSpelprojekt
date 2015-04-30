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

	void init(bool is3D, int file, glm::vec3 pos, float dist, int loopType, int loopInterval);
	void update(float deltaTime);
	void bindToPosition(glm::vec3* pos, bool track);

private:


public:

private:
	ALuint* sourcePointer; //pointer to gameObject position for audio to follow it
	int fileId, state;
	glm::vec3 position;
	glm::vec3* objectPosition; //used for tracking and following a gameobject
	bool tracking;
	bool play3D;
	float distance;
	bool looping;
	bool intervalLooping;
	float time, timeSinceLastPlayed;
	float interval; //used for playing non looping sounds after a random interval

};

#endif