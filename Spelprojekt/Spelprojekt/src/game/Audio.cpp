#include "Audio.h"
#include <iostream>

using namespace std;

Audio::Audio()
{
	currTrack = -1;

	////music
	//for (int i = 0; i < MUSIC_FILES; i++)
	//{
	//	music[i] = new AudioObject;
	//	music[i]->volume = 0.0;
	//}

	//load music files
	musicFiles[0] = "../Audio/Music/witcher_dusk.wav";
	musicFiles[1] = "../Audio/Music/witcher_omnious.wav";
	musicFiles[2] = "../Audio/Music/witcher_battle.wav";

	//load sound files
	//player
	soundFiles[0] = "../Audio/Sounds/Shrine/rune_received.wav";
	soundFiles[1] = "../Audio/Sounds/Player/player_resurrected.wav";
	soundFiles[2] = "../Audio/Sounds/Player/player_attack_miss.wav";
	soundFiles[3] = "../Audio/Sounds/Player/player_attack_fire.wav";
	soundFiles[4] = "../Audio/Sounds/Player/player_attack_ice.wav";
	soundFiles[5] = "../Audio/Sounds/Player/player_shield_force.wav";
	//interface
	soundFiles[6] = "../Audio/Sounds/Interface/button.wav";
	soundFiles[7] = "../Audio/Sounds/Interface/pause.wav";
	//enemies and bosses
	soundFiles[8] = "../Audio/Sounds/Bosses/boss_clear.wav";
	//ambient
	soundFiles[9] = "../Audio/Sounds/Ambient/ambient_water_drop.wav";
	//...
}

Audio::~Audio()
{

}

bool Audio::init()
{
	//Init OpenAL
	device = alcOpenDevice(NULL);
	if (!device) return endWithError("no sound device");
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	if (!context) return endWithError("no sound context");

	//alGenSources(1, &music[0]->source);

	//Listener
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
	//First direction vector, then vector pointing up)                                                                                
	alListenerfv(AL_POSITION, ListenerPos); //Set position of the listener
	alListenerfv(AL_VELOCITY, ListenerVel); //Set velocity of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri);

	// create music buffers
	alGenBuffers(MUSIC_BUFFERS, musicBuffer);
	createBuffers(musicFiles, musicBuffer, MUSIC_BUFFERS);
	//bindSourceToBuffer(music[0]->source, musicBuffer[0]); //remove this after playMusic works

	// create sound buffers
	alGenBuffers(SOUND_BUFFERS, soundBuffer);
	createBuffers(soundFiles, soundBuffer, SOUND_BUFFERS);

	return EXIT_SUCCESS;
}

bool Audio::loadAudio(AudioObject* audioObj) //old function remove this later
{
	//FILE *fp = NULL;
	////fp = fopen(audioObj->file, "rb");
	//fp = fopen(soundFiles[0], "rb");
	//if (!fp) return endWithError("Failed to open file");

	//WaveHeader waveFileHeader;

	////Check that the WAVE file is OK
	//fread(waveFileHeader.type, sizeof(char), 4, fp);                                              //Reads the first bytes in the file
	//if (waveFileHeader.type[0] != 'R' || waveFileHeader.type[1] != 'I'
	//	|| waveFileHeader.type[2] != 'F' || waveFileHeader.type[3] != 'F')            //Should be "RIFF"
	//	return endWithError("No RIFF");                                            //Not RIFF

	//fread(&waveFileHeader.size, sizeof(unsigned long), 1, fp);                                           //Continue to read the file
	//fread(waveFileHeader.type, sizeof(char), 4, fp);                                             //Continue to read the file
	//if (waveFileHeader.type[0] != 'W' || waveFileHeader.type[1] != 'A'
	//	|| waveFileHeader.type[2] != 'V' || waveFileHeader.type[3] != 'E')           //This part should be "WAVE"
	//	return endWithError("not WAVE");                                            //Not WAVE

	//fread(waveFileHeader.type, sizeof(char), 4, fp);                                              //Continue to read the file
	//if (waveFileHeader.type[0] != 'f' || waveFileHeader.type[1] != 'm'
	//	|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != ' ')           //This part should be "fmt "
	//	return endWithError("not fmt ");                                            //Not fmt 

	////Now we know that the file is a acceptable WAVE file
	////Info about the WAVE data is now read and stored
	//fread(&waveFileHeader.chunkSize, sizeof(unsigned long), 1, fp);
	//fread(&waveFileHeader.formatType, sizeof(short), 1, fp);
	//fread(&waveFileHeader.channels, sizeof(short), 1, fp);
	//fread(&waveFileHeader.sampleRate, sizeof(unsigned long), 1, fp);
	//fread(&waveFileHeader.avgBytesPerSec, sizeof(unsigned long), 1, fp);
	//fread(&waveFileHeader.bytesPerSample, sizeof(short), 1, fp);
	//fread(&waveFileHeader.bitsPerSample, sizeof(short), 1, fp);

	//fread(waveFileHeader.type, sizeof(char), 4, fp);
	//if (waveFileHeader.type[0] != 'd' || waveFileHeader.type[1] != 'a'
	//	|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != 'a')           //This part should be "data"
	//	return endWithError("Missing DATA");                                        //not data

	//fread(&waveFileHeader.dataSize, sizeof(unsigned long), 1, fp);                                        //The size of the sound data is read

	////Display the info about the WAVE file
	////cout << "Chunk Size: " << waveFileHeader.chunkSize << "\n";
	////cout << "Format Type: " << waveFileHeader.formatType << "\n";
	////cout << "Channels: " << waveFileHeader.channels << "\n";
	////cout << "Sample Rate: " << waveFileHeader.sampleRate << "\n";
	////cout << "Average Bytes Per Second: " << waveFileHeader.avgBytesPerSec << "\n";
	////cout << "Bytes Per Sample: " << waveFileHeader.bytesPerSample << "\n";
	////cout << "Bits Per Sample: " << waveFileHeader.bitsPerSample << "\n";
	////cout << "Data Size: " << waveFileHeader.dataSize << "\n";

	//unsigned char* buf = new unsigned char[waveFileHeader.dataSize];                            //Allocate memory for the sound data
	//cout << fread(buf, sizeof(char), waveFileHeader.dataSize, fp) << " bytes loaded\n";           //Read the sound data and display the 
	////number of bytes loaded.
	////Should be the same as the Data Size if OK

	////Stores the sound data
	//ALuint frequency = waveFileHeader.sampleRate;;                                               //The Sample Rate of the WAVE file
	//ALenum format = 0;                                                            //The audio format (bits per sample, number of channels)
	//ALint state;

	//alGenBuffers(1, &audioObj->buffer);                                                    //Generate one OpenAL musicBuffer and link to "musicBuffer"
	//alGenSources(1, &audioObj->source);                                                   //Generate one OpenAL musicSource and link to "musicSource"
	//if (alGetError() != AL_NO_ERROR) return endWithError("Error GenSource");     //Error during musicBuffer/musicSource generation

	////Figure out the format of the WAVE file
	//if (waveFileHeader.bitsPerSample == 8)
	//{
	//	if (waveFileHeader.channels == 1)
	//		format = AL_FORMAT_MONO8;
	//	else if (waveFileHeader.channels == 2)
	//		format = AL_FORMAT_STEREO8;
	//}
	//else if (waveFileHeader.bitsPerSample == 16)
	//{
	//	if (waveFileHeader.channels == 1)
	//		format = AL_FORMAT_MONO16;
	//	else if (waveFileHeader.channels == 2)
	//		format = AL_FORMAT_STEREO16;
	//}
	//if (!format) return endWithError("Wrong BitPerSample");                      //Not valid format

	//alBufferData(audioObj->buffer, format, buf, waveFileHeader.dataSize, frequency);                    //Store the sound data in the OpenAL musicBuffer
	//if (alGetError() != AL_NO_ERROR)
	//	return endWithError("Error loading ALBuffer");                              //Error during musicBuffer loading

	////Sound setting variables
	//ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the musicSource sound
	//ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                 //Set orientation of the listener

	////musicSource
	//alSourcei(audioObj->source, AL_BUFFER, audioObj->buffer);                                 //Link the musicBuffer to the musicSource
	//alSourcef(audioObj->source, AL_PITCH, 1.0f);                                 //Set the pitch of the musicSource
	//alSourcef(audioObj->source, AL_GAIN, VOLUME_MAX);                                 //Set the gain of the musicSource
	//alSourcefv(audioObj->source, AL_POSITION, SourcePos);                                 //Set the position of the musicSource
	//alSourcefv(audioObj->source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the musicSource
	//alSourcei(audioObj->source, AL_LOOPING, audioObj->looping);                                 //Set if musicSource is looping sound

	//audioObj->state = A_NOT_PLAYING;

	////Clean-up
	//fclose(fp);
	//delete[] buf;

	return EXIT_SUCCESS;
}

bool Audio::createBuffers(char** files, ALuint* buffers, int elements)
{
	for (int i = 0; i < elements; i++)
	{
		FILE *fp = NULL;
		fp = fopen(files[i], "rb"); // open audio file for reading
		if (!fp) return endWithError("Failed to open file");

		WaveHeader waveFileHeader;

		//Check that the WAVE file is OK
		fread(waveFileHeader.type, sizeof(char), 4, fp);
		if (waveFileHeader.type[0] != 'R' || waveFileHeader.type[1] != 'I'  // check RIFF
			|| waveFileHeader.type[2] != 'F' || waveFileHeader.type[3] != 'F')
			return endWithError("No RIFF");

		fread(&waveFileHeader.size, sizeof(unsigned long), 1, fp);
		fread(waveFileHeader.type, sizeof(char), 4, fp);
		if (waveFileHeader.type[0] != 'W' || waveFileHeader.type[1] != 'A' // check WAVE
			|| waveFileHeader.type[2] != 'V' || waveFileHeader.type[3] != 'E')
			return endWithError("not WAVE");

		fread(waveFileHeader.type, sizeof(char), 4, fp);
		if (waveFileHeader.type[0] != 'f' || waveFileHeader.type[1] != 'm' // check FMT
			|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != ' ')
			return endWithError("not fmt ");

		//Now we know that the file is a acceptable WAVE file
		//Info about the WAVE data is now read and stored
		fread(&waveFileHeader.chunkSize, sizeof(unsigned long), 1, fp);
		fread(&waveFileHeader.formatType, sizeof(short), 1, fp);
		fread(&waveFileHeader.channels, sizeof(short), 1, fp);
		fread(&waveFileHeader.sampleRate, sizeof(unsigned long), 1, fp);
		fread(&waveFileHeader.avgBytesPerSec, sizeof(unsigned long), 1, fp);
		fread(&waveFileHeader.bytesPerSample, sizeof(short), 1, fp);
		fread(&waveFileHeader.bitsPerSample, sizeof(short), 1, fp);

		fread(waveFileHeader.type, sizeof(char), 4, fp);
		if (waveFileHeader.type[0] != 'd' || waveFileHeader.type[1] != 'a' // check data
			|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != 'a')
			return endWithError("Missing DATA");

		fread(&waveFileHeader.dataSize, sizeof(unsigned long), 1, fp);

		unsigned char* buf = new unsigned char[waveFileHeader.dataSize];                            //Allocate memory for the sound data
		cout << fread(buf, sizeof(char), waveFileHeader.dataSize, fp) << " bytes loaded\n";           //Read the sound data and display the 
		//number of bytes loaded.
		//Should be the same as the Data Size if OK

		//Stores the sound data
		ALuint frequency = waveFileHeader.sampleRate;                                               //The Sample Rate of the WAVE file
		ALenum format = 0;                                                            //The audio format (bits per sample, number of channels)
		ALint state;

		//Figure out the format of the WAVE file
		if (waveFileHeader.bitsPerSample == 8)
		{
			if (waveFileHeader.channels == 1)
				format = AL_FORMAT_MONO8;
			else if (waveFileHeader.channels == 2)
				format = AL_FORMAT_STEREO8;
		}
		else if (waveFileHeader.bitsPerSample == 16)
		{
			if (waveFileHeader.channels == 1)
				format = AL_FORMAT_MONO16;
			else if (waveFileHeader.channels == 2)
				format = AL_FORMAT_STEREO16;
		}
		if (!format) return endWithError("Wrong BitPerSample");                      //Not valid format

		alBufferData(buffers[i], format, buf, waveFileHeader.dataSize, frequency);                    //Store the sound data in the OpenAL buffers
		if (alGetError() != AL_NO_ERROR)
			return endWithError("Error loading ALBuffer");

		//Clean-up
		fclose(fp);
		delete[] buf;
	}

	return EXIT_SUCCESS;
}

bool Audio::bindSourceToBuffer(ALuint source, ALuint buffer)
{
	//sound position
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the musicSource sound
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                 //Set orientation of the listener

	//musicSource
	alSourcei(source, AL_BUFFER, buffer);                                 //Link the musicBuffer to the musicSource
	alSourcef(source, AL_PITCH, 1.0f);                                 //Set the pitch of the musicSource
	alSourcef(source, AL_GAIN, VOLUME_MAX);                                 //Set the gain of the musicSource
	alSourcefv(source, AL_POSITION, SourcePos);                                 //Set the position of the musicSource
	alSourcefv(source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the musicSource
	alSourcei(source, AL_LOOPING, AL_TRUE);

	alSourcePlay(source);

	return EXIT_SUCCESS;
}

void Audio::update(float deltaTime)
{
	//music
	//for (int i = 0; i < MUSIC_FILES; i++)
	//{
	//	if (music[i]->state == A_FADEIN)
	//	{
	//		music[i]->volume += FADEINTIME * deltaTime;
	//		if (music[i]->volume >= VOLUME_MAX)
	//		{
	//			music[i]->state = A_PLAYING;
	//			music[i]->volume = VOLUME_MAX;
	//		}
	//		alSourcef(music[i]->source, AL_GAIN, music[i]->volume);
	//	}

	//	else if (music[i]->state == A_FADEOUT)
	//	{
	//		music[i]->volume -= FADEOUTTIME * deltaTime;
	//		if (music[i]->volume <= 0.0f)
	//		{
	//			music[i]->state = A_NOT_PLAYING;
	//			music[i]->volume = 0.0f;
	//			stopMusic(i);
	//		}
	//		alSourcef(music[i]->source, AL_GAIN, music[i]->volume);
	//	}
	//}

	////sounds
	//for (int i = 0; i < SOUND_FILES; i++)
	//{
	//	if (sounds[i]->state == A_PLAYING)
	//	{
	//		ALint state;
	//		alGetSourcei(sounds[i]->source, AL_SOURCE_STATE, &state);
	//		if (state != AL_PLAYING)
	//			sounds[i]->state = A_NOT_PLAYING;
	//	}
	//}

	for (int i = 0; i < soundSources.size(); i++)
	{
		ALint state;
		alGetSourcei(soundSources[i], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
			soundSources.erase(soundSources.begin() + i);
	}
	printf("Num buffers: %i\n", soundSources.size());

}

void Audio::playMusic(int track)
{
	/*if (track < MUSIC_FILES && track >= 0)
	{
		if (music[track]->state == A_NOT_PLAYING)
		{
			stopMusic(currTrack);
			if (track != -1)
			{
				music[track]->state = A_PLAYING;
				alSourcePlay(music[track]->source);
			}
			currTrack = track;
		}
	}
	else
	{
		stopMusic(currTrack);
	}	*/
}

void Audio::playMusicFade(int track, float deltaTime)
{
	//if (track < MUSIC_FILES && track >= 0)
	//{
	//	if (currTrack != track)//if not current track
	//	{
	//		oldTrack = currTrack;
	//		music[oldTrack]->state = A_FADEOUT;//fade out old music
	//		music[track]->state = A_FADEIN;//fade in new music
	//		alSourcePlay(music[track]->source);
	//		currTrack = track;
	//	}
	//	else if (currTrack == track)//check if currently fading in or out
	//	{
	//		if (music[track]->state == A_FADEOUT)
	//			music[track]->state = A_FADEIN;
	//	}
	//}
	//else //if out of bounds
	//{
	//	music[currTrack]->state = A_FADEOUT;
	//	music[oldTrack]->state = A_FADEOUT;
	//}
}

void Audio::stopMusic(int track)
{
	/*if (track >= 0)
	{
		music[track]->state = A_NOT_PLAYING;
		alSourceStop(music[track]->source);
	}
*/
}

void Audio::playSound(int file)
{
	if (file < SOUND_BUFFERS)
	{
		ALuint source;
		alGenSources(1, &source);

		ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the musicSource sound
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcei(source, AL_BUFFER, soundBuffer[file]);                                 //Link the musicBuffer to the musicSource
		alSourcef(source, AL_PITCH, 1.0f);                                 //Set the pitch of the musicSource
		alSourcef(source, AL_GAIN, VOLUME_MAX);  
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
		alSourcefv(source, AL_POSITION, SourcePos);                                 //Set the position of the musicSource
		alSourcefv(source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the musicSource
		alSourcei(source, AL_LOOPING, AL_FALSE);

		alSourcePlay(source);

		soundSources.push_back(source);
	}
}

void Audio::playSoundAtPos(int file, glm::vec2 pos, bool looping)
{
		if (file < SOUND_BUFFERS)
		{
			ALuint source;
			alGenSources(1, &source);

			ALfloat SourcePos[] = { pos.x, pos.y, 0.0 };                                    //Position of the musicSource sound
			ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

			alSourcei(source, AL_BUFFER, soundBuffer[file]);                                 //Link the musicBuffer to the musicSource
			alSourcef(source, AL_PITCH, 1.0f);                                 //Set the pitch of the musicSource
			alSourcef(source, AL_GAIN, VOLUME_MAX);
			alSourcefv(source, AL_POSITION, SourcePos);                                 //Set the position of the musicSource
			alSourcefv(source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the musicSource
			alSourcei(source, AL_LOOPING, looping);

			alSourcePlay(source);

			soundSources.push_back(source);
		}
}

void Audio::updateListener(glm::vec3 pos)
{
	ALfloat listenerPos[] = { pos.x, pos.y, pos.z };
	ALfloat musicPos[] = { pos.x, pos.y, pos.z };

	alListenerfv(AL_POSITION, listenerPos);

	////music
	//for (int i = 0; i < MUSIC_FILES; i++)
	//	alSourcefv(music[i]->source, AL_POSITION, musicPos);

	////non env sounds
	//for (int i = 0; i < SOUND_FILES; i++)
	//	alSourcefv(sounds[i]->source, AL_POSITION, musicPos);
}

int Audio::getTrack()
{
	return currTrack;
}

void Audio::shutdown()
{
	// music
	//for (int i = 0; i < MUSIC_FILES; i++)
	//{
	//	alDeleteSources(1, &music[i]->source);
	//	//alDeleteBuffers(1, &music[i]->buffer);
	//	delete music[i];
	//}

	alDeleteBuffers(MUSIC_BUFFERS, musicBuffer);

	// sounds
	//for (int j = 0; j < SOUND_FILES; j++)
	//{
	//	alDeleteSources(1, &sounds[j]->source);
	//	//alDeleteBuffers(1, &sounds[j]->buffer);
	//	delete sounds[j];
	//}

	soundSources.clear();
	alDeleteBuffers(SOUND_BUFFERS, soundBuffer);

	alcDestroyContext(context);
	alcCloseDevice(device);
}

int Audio::endWithError(char* msg, int error)
{
	//Display error message in console
	cout << msg << "\n";
	system("PAUSE");
	return error;
}
