#ifndef AUDIO_H
#define AUDIO_H

#include <conio.h>
#include <stdlib.h>
#include <vector>

#include <..\glm\glm.hpp>
#include <AL\alut.h>

//states
#define A_NOT_PLAYING 0
#define A_PLAYING 1
#define A_PAUSED 2
#define A_FADEIN 3
#define A_FADEOUT 4
#define FADEINTIME 0.15f
#define FADEOUTTIME 0.4f
#define VOLUME_MAX 0.5f // goes from 0.0 to 1.0
#define MUSIC_BUFFERS 3
#define SOUND_BUFFERS 10

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

	struct AudioObject
	{
		int state;
		ALuint source;
		ALfloat volume;
	};


public:
	Audio();
	~Audio();
	//ALboolean LoadALData();
	bool init();
	void update(float deltaTime);
	void playMusic(int track);
	void playMusicFade(int track, float deltaTime);
	void stopMusic(int track);
	void playSound(int track);
	void playSoundAtPos(int file, glm::vec2 pos, bool looping);
	void createSoundAtPos(int track, glm::vec2 pos);
	void updateListener(glm::vec3 pos);
	int	getTrack();
	void shutdown();

private:
	bool loadAudio(AudioObject* audioObj);
	bool createBuffers(char** files, ALuint* buffers, int elements);
	bool bindSourceToBuffer(ALuint source, ALuint buffer);
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

	//object arrays
	//AudioObject* music[MUSIC_FILES];
	//AudioObject* sounds[SOUND_FILES];

	//misc vars
	int currTrack, oldTrack;
};

#endif