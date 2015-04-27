#ifndef AUDIO_H
#define AUDIO_H

#include <conio.h>
#include <stdlib.h>
#include <vector>

#include <..\glm\glm.hpp>
#include <AL\alut.h>

//states
#define A_PLAYING 0
#define A_PAUSED 1
#define A_FADEIN 2
#define A_FADEOUT 3
#define FADEINTIME 0.15f
#define FADEOUTTIME 0.4f
#define MUSIC_VOLUME 0.5f // goes from 0.0 to 1.0
#define SOUND_VOLUME 1.0f // goes from 0.0 to 1.0
#define MASTER_VOLUME 0.5f // goes from 0.0 to 1.0
#define MUSIC_BUFFERS 3 // one buffer for each file
#define SOUND_BUFFERS 10 // one buffer for each file

using namespace std;

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

	struct MusicStruct
	{
		int state;
		int track;
		ALuint source;
		ALfloat volume;
	};


public:
	Audio();
	~Audio();
	//ALboolean LoadALData();
	bool init();
	void update(float deltaTime);
	void playMusic(int file);
	void playMusicFade(int file, float deltaTime);
	void playSound(int file);
	void playSoundAtPos(int file, glm::vec2 pos, bool looping);
	void updateListener(glm::vec3 pos);
	void shutdown();

private:
	bool createBuffers(char** files, ALuint* buffers, int elements);
	int endWithError(char* msg, int error = 0);

private:
	//OpenAL device and context
	ALCdevice *device;                                                          //Create an OpenAL Device
	ALCcontext *context;

	//files
	char* musicFiles[MUSIC_BUFFERS];
	char* soundFiles[SOUND_BUFFERS];

	//buffers
	ALuint musicBuffer[MUSIC_BUFFERS];
	ALuint soundBuffer[SOUND_BUFFERS];

	// audio source lists
	vector<ALuint> soundSources;
	vector<MusicStruct> musicSources;

	//misc vars //remove these later
	int currTrack, oldTrack;
};

#endif