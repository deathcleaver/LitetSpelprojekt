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

	void init(bool is3D, int file, glm::vec3 pos, float dist, int loopType, float minInt, float maxInt);
	void update(float deltaTime);
	void setPosition(glm::vec3 pos);
	void setPitch(float pitch);
	void stopSound();

	void getAllValues(int &type, int &id, float &posX, float &posY, float &posZ, float &dist, int &loopType, float &minInterval, float &maxInterval);
private:


public:

private:
	ALuint sourcePointer; //pointer to gameObject position for audio to follow it
	int fileId, state;
	glm::vec3 position;
	glm::vec3* objectPosition; //used for tracking and following a gameobject
	bool tracking;
	bool play3D; //0 false, 1 true
	float distance; //audible distance of the sound
	bool looping;
	bool intervalLooping; //used for playing non looping sounds after a random interval
	// set minInterval = maxInterval for non random interval
	float time, nextInstance;
	float minInterval, maxInterval;

};

#endif