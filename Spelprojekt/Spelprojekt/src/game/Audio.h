#ifndef AUDIO_H
#define AUDIO_H

#include <conio.h>
#include <stdlib.h>

#include <AL\alut.h>

class Audio
{
public:
	Audio();
	~Audio();
	//ALboolean LoadALData();
	bool playSound();
	void shutdown();

private:
	int endWithError(char* msg, int error = 0);
	//ALuint buffer;
	//ALuint source;

	//// source sound pos
	//ALfloat sourcePos[3] = { 0.0, 0.0, 0.0 };
	//ALfloat sourceVel[3] = { 0.0, 0.0, 0.0 };
	//ALfloat listenerPos[3] = { 0.0, 0.0, 0.0 };
	//ALfloat sourceVel[3] = { 0.0, 0.0, 0.0 };
	//ALfloat listenerOri[6] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };







	
};

#endif