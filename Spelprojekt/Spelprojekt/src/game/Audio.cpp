#include "Audio.h"
#include <iostream>

using namespace std;

Audio::Audio()
{
	currTrack = -1;
	musicTracks[0] = "../Audio/Music/witcher_dusk.wav";
	musicTracks[1] = "../Audio/Music/witcher_omnious.wav";
	musicTracks[2] = "../Audio/Music/witcher_battle.wav";
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

	loadMusic(0);
	loadMusic(1);
	loadMusic(2);

	return EXIT_SUCCESS;
}

bool Audio::loadMusic(int fileId)
{
	//Loading of the WAVE file
	FILE *fp = NULL;
	fp = fopen(musicTracks[fileId], "rb");
	if (!fp) return endWithError("Failed to open file");

	WaveHeader waveFileHeader;

	//Check that the WAVE file is OK
	fread(waveFileHeader.type, sizeof(char), 4, fp);                                              //Reads the first bytes in the file
	if (waveFileHeader.type[0] != 'R' || waveFileHeader.type[1] != 'I'
		|| waveFileHeader.type[2] != 'F' || waveFileHeader.type[3] != 'F')            //Should be "RIFF"
		return endWithError("No RIFF");                                            //Not RIFF

	fread(&waveFileHeader.size, sizeof(unsigned long), 1, fp);                                           //Continue to read the file
	fread(waveFileHeader.type, sizeof(char), 4, fp);                                             //Continue to read the file
	if (waveFileHeader.type[0] != 'W' || waveFileHeader.type[1] != 'A'
		|| waveFileHeader.type[2] != 'V' || waveFileHeader.type[3] != 'E')           //This part should be "WAVE"
		return endWithError("not WAVE");                                            //Not WAVE

	fread(waveFileHeader.type, sizeof(char), 4, fp);                                              //Continue to read the file
	if (waveFileHeader.type[0] != 'f' || waveFileHeader.type[1] != 'm'
		|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != ' ')           //This part should be "fmt "
		return endWithError("not fmt ");                                            //Not fmt 

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
	if (waveFileHeader.type[0] != 'd' || waveFileHeader.type[1] != 'a'
		|| waveFileHeader.type[2] != 't' || waveFileHeader.type[3] != 'a')           //This part should be "data"
		return endWithError("Missing DATA");                                        //not data

	fread(&waveFileHeader.dataSize, sizeof(unsigned long), 1, fp);                                        //The size of the sound data is read

	//Display the info about the WAVE file
	cout << "Chunk Size: " << waveFileHeader.chunkSize << "\n";
	cout << "Format Type: " << waveFileHeader.formatType << "\n";
	cout << "Channels: " << waveFileHeader.channels << "\n";
	cout << "Sample Rate: " << waveFileHeader.sampleRate << "\n";
	cout << "Average Bytes Per Second: " << waveFileHeader.avgBytesPerSec << "\n";
	cout << "Bytes Per Sample: " << waveFileHeader.bytesPerSample << "\n";
	cout << "Bits Per Sample: " << waveFileHeader.bitsPerSample << "\n";
	cout << "Data Size: " << waveFileHeader.dataSize << "\n";

	unsigned char* buf = new unsigned char[waveFileHeader.dataSize];                            //Allocate memory for the sound data
	cout << fread(buf, sizeof(char), waveFileHeader.dataSize, fp) << " bytes loaded\n";           //Read the sound data and display the 
	//number of bytes loaded.
	//Should be the same as the Data Size if OK

	//Stores the sound data
	ALuint frequency = waveFileHeader.sampleRate;;                                               //The Sample Rate of the WAVE file
	ALenum format = 0;                                                            //The audio format (bits per sample, number of channels)
	ALint state;

	alGenBuffers(1, &musicBuffer[fileId]);                                                    //Generate one OpenAL musicBuffer and link to "musicBuffer"
	alGenSources(1, &musicSource[fileId]);                                                   //Generate one OpenAL musicSource and link to "musicSource"
	if (alGetError() != AL_NO_ERROR) return endWithError("Error GenSource");     //Error during musicBuffer/musicSource generation

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

	alBufferData(musicBuffer[fileId], format, buf, waveFileHeader.dataSize, frequency);                    //Store the sound data in the OpenAL musicBuffer
	if (alGetError() != AL_NO_ERROR)
		return endWithError("Error loading ALBuffer");                              //Error during musicBuffer loading

	//Sound setting variables
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the musicSource sound
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                    //Velocity of the musicSource sound
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };                                  //Position of the listener
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                                  //Velocity of the listener
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };                 //Orientation of the listener
	//First direction vector, then vector pointing up) 
	//Listener                                                                               
	alListenerfv(AL_POSITION, ListenerPos);                                  //Set position of the listener
	alListenerfv(AL_VELOCITY, ListenerVel);                                  //Set velocity of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri);                                  //Set orientation of the listener

	//musicSource
	alSourcei(musicSource[fileId], AL_BUFFER, musicBuffer[fileId]);                                 //Link the musicBuffer to the musicSource
	alSourcef(musicSource[fileId], AL_PITCH, 1.0f);                                 //Set the pitch of the musicSource
	alSourcef(musicSource[fileId], AL_GAIN, 1.0f);                                 //Set the gain of the musicSource
	alSourcefv(musicSource[fileId], AL_POSITION, SourcePos);                                 //Set the position of the musicSource
	alSourcefv(musicSource[fileId], AL_VELOCITY, SourceVel);                                 //Set the velocity of the musicSource
	alSourcei(musicSource[fileId], AL_LOOPING, AL_TRUE);                                 //Set if musicSource is looping sound

	//Clean-up
	fclose(fp);
	delete[] buf;
	
	return EXIT_SUCCESS;
}

void Audio::playMusic(int track)
{
	if (currTrack != track)
	{
		stopMusic(currTrack);
		alSourcePlay(musicSource[track]);
		currTrack = track;
	}
}

void Audio::playSound()
{
	// test code
	//alSourcePlay(musicSource);
}

void Audio::updateListener(glm::vec3 pos)
{
	ALfloat listenerPos[] = { pos.x, pos.y, pos.z };
	ALfloat SourcePos[] = { pos.x, pos.y, pos.z };
	alSourcefv(musicSource[currTrack], AL_POSITION, SourcePos);
	alListenerfv(AL_POSITION, listenerPos);
}

void Audio::stopMusic(int track)
{
	alSourceStop(musicSource[track]);
}

void Audio::shutdown()
{
	for (int i = 0; i < numOfTracks; i++)
	{
		alDeleteSources(1, &musicSource[i]);
		alDeleteBuffers(1, &musicBuffer[i]);
	}
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
