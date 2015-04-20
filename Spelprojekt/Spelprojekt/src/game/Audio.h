#ifndef AUDIO_H
#define AUDIO_H

#include <conio.h>
#include <stdlib.h>

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
	bool LoadMusic(int fileId);
	void  playSound();
	void shutdown();

private:
	int endWithError(char* msg, int error = 0);
	
private:
	ALCdevice *device;                                                          //Create an OpenAL Device
	ALCcontext *context;

	char* musicTracks[2];
	int currTrack;

	//source and buffers
	ALuint source;
	ALuint buffer;


	//ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	//ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
	//ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	//ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	//ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };	
};

#endif