#ifndef AUDIO_H
#define AUDIO_H

#include <conio.h>
#include <stdlib.h>

#include <..\glm\glm.hpp>
#include <AL\alut.h>

class Audio
{
	struct WaveHeader
	{
		char type[4];
		unsigned long size, chunkSize;
		short formatType, channels;
		unsigned long sampleRate, avgBytesPerSec;
		short bytesPerSample, bitsPerSample;
		unsigned long dataSize;
	};

public:
	Audio();
	~Audio();
	//ALboolean LoadALData();
	bool init();
	bool loadMusic(int fileId);
	void playMusic(int track);
	void playSound();
	void updateListener(glm::vec3 pos);
	void stopMusic(int track);
	void shutdown();

private:
	bool fadeMusic(bool fadeToZero);
	int endWithError(char* msg, int error = 0);
	
private:
	ALCdevice *device;                                                          //Create an OpenAL Device
	ALCcontext *context;

	int numOfTracks = 3;
	char* musicTracks[3];
	int currTrack, newTrack;
	float maxVolume;
	float currVolume;
	float fadeVolume;

	//source and buffers
	ALuint musicSource[3];
	ALuint musicBuffer[3];


	//ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	//ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
	//ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	//ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	//ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };	
};

#endif