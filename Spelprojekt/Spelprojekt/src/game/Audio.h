#ifndef AUDIO_H
#define AUDIO_H

#include <conio.h>
#include <stdlib.h>

#include <..\glm\glm.hpp>
#include <AL\alut.h>

class Audio
{
private:
	struct WaveHeader
	{
		char type[4];
		unsigned long size, chunkSize;
		short formatType, channels;
		unsigned long sampleRate, avgBytesPerSec;
		short bytesPerSample, bitsPerSample;
		unsigned long dataSize;
	};

	struct AudioObject
	{
		ALuint source;
		ALuint buffer;
		char* file;
		float volume;
		bool looping;
	};


public:
	Audio();
	~Audio();
	//ALboolean LoadALData();
	bool init();
	bool loadAudio(AudioObject* audioObj);
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

	AudioObject music[3];
	AudioObject sounds[1];

	int numOfTracks = 3;
	int currTrack, oldTrack;
	float maxVolume;
	float currVolume, oldVolume;
	float fadeCurrVol, fadeNextVol;
};

#endif