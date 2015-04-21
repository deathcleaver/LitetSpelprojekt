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
	bool loadAudio(int fileId, ALuint source, ALuint buffer);
	void playMusic(int track);
	void playMusicFade(int track, float deltaTime);
	void playSound();
	void updateListener(glm::vec3 pos);
	void stopMusic(int track);
	int	getTrack();
	void shutdown();

private:
	bool fadeMusic(float deltaTime);
	int endWithError(char* msg, int error = 0);
	
private:
	ALCdevice *device;                                                          //Create an OpenAL Device
	ALCcontext *context;

	int numOfTracks = 3;
	char* musicTracks[3];
	char* soundTracks[1];
	int currTrack, oldTrack;
	float maxVolume;
	float currVolume, oldVolume;
	float fadeCurrVol, fadeNextVol;

	//source and buffers
	//music
	ALuint musicSource[3];
	ALuint musicBuffer[3];

	//sound
	ALuint soundSource[1];
	ALuint soundBuffer[1];
};

#endif