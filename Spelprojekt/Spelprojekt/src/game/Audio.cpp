#include "Audio.h"
#include <iostream>

#include "../DebugLog.h"

using namespace std;

Audio& Audio::getAudio()
{
	static Audio singleton;
	return singleton;
}

Audio::Audio()
{
	
}

Audio::~Audio()
{
	shutdown();
}

bool Audio::init(float musicV, float soundV, float masterV, bool musicE, bool soundE, bool audioE)
{
	//init options
	applySettings(musicV, soundV, masterV, musicE, soundE, audioE);

	//load tracks
	loadFiles();

	//Init OpenAL
	device = alcOpenDevice(NULL);
	if (!device) return endWithError("no sound device");
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	if (!context) return endWithError("no sound context");

	//Listener
	listenerPos[0] = 0.0;
	listenerPos[1] = 0.0;
	listenerPos[2] = 0.0;
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 }; //up, lookat                                                                                
	alListenerfv(AL_POSITION, listenerPos); //Set position of the listener
	alListenerfv(AL_VELOCITY, ListenerVel); //Set velocity of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri);

	// create music buffers
	alGenBuffers(MUSIC_BUFFERS, musicBuffer);
	createBuffers(musicFiles, musicBuffer, MUSIC_BUFFERS);

	// create sound buffers
	alGenBuffers(SOUND_BUFFERS, soundBuffer);
	createBuffers(soundFiles, soundBuffer, SOUND_BUFFERS);

	return EXIT_SUCCESS;
}

void Audio::applySettings(float musicV, float soundV, float masterV, bool musicE, bool soundE, bool audioE)
{
	if (musicV > 1.0f) // music
		musicVolume = 1.0f;
	else
		musicVolume = musicV;

	if (soundV > 1.0f) // sound
		soundVolume = 1.0f;
	else
		soundVolume = soundV;

	if (masterV > 1.0f) // master
		masterVolume = 1.0f;
	else
		masterVolume = masterV;

	musicEnabled = musicE;
	soundEnabled = soundE;
	audioEnabled = audioE;
}

void Audio::toggleAudio()
{
	if (audioEnabled)
		audioEnabled = false;
	else
		audioEnabled = true;
}

void Audio::toggleMusic()
{
	if (musicEnabled)
		musicEnabled = false;
	else
		musicEnabled = true;
}

void Audio::toggleSound()
{
	if (soundEnabled)
		soundEnabled = false;
	else
		soundEnabled = true;
}

void Audio::loadFiles()
{
	//load music files
	musicFiles[0] = "../Audio/Music/witcher_dusk.wav";
	musicFiles[1] = "../Audio/Music/witcher_cave.wav"; // mining area
	musicFiles[2] = "../Audio/Music/witcher_battle_bat.wav"; // boss battles bat
	musicFiles[3] = "../Audio/Music/mine_track.wav"; // awesome room mine
	musicFiles[4] = "../Audio/Music/witcher_dike.wav"; // mushroom area
	musicFiles[5] = "../Audio/Music/witcher_omnious.wav"; // crypt area
	musicFiles[6] = "../Audio/Music/witcher_battle_spider.wav"; // boss battles spider
	musicFiles[7] = "../Audio/Music/witcher_nocturnal.wav"; // ghost area
	musicFiles[8] = "../Audio/Music/witcher_battle_ghost.wav"; // boss battle ghost
	musicFiles[9] = "../Audio/Music/grim_stage1.wav";
	musicFiles[10] = "../Audio/Music/Grim/grim_stage2_intro.wav";
	musicFiles[11] = "../Audio/Music/Grim/grim_stage2_loop.wav";
	musicFiles[12] = "../Audio/Music/witcher_credits.wav";

	//load sound files
	//Shrine
	soundFiles[0] = "../Audio/Sounds/Shrine/rune_received.wav";
	soundFiles[1] = "../Audio/Sounds/Shrine/player_healed.wav";
	soundFiles[51] = "../Audio/Sounds/Shrine/player_saved.wav";
	//player
	soundFiles[2] = "../Audio/Sounds/Player/player_resurrected.wav";
	soundFiles[3] = "../Audio/Sounds/Player/player_attack_miss.wav";
	soundFiles[4] = "../Audio/Sounds/Player/player_attack_fire.wav";
	soundFiles[5] = "../Audio/Sounds/Player/player_attack_ice.wav";
	soundFiles[6] = "../Audio/Sounds/Player/player_shield_force.wav";
	soundFiles[7] = "../Audio/Sounds/Player/player_landing.wav";
	soundFiles[26] = "../Audio/Sounds/Player/player_hurt.wav";
	soundFiles[61] = "../Audio/Sounds/Player/player_mirror_walk.wav";
	//interface
	soundFiles[8] = "../Audio/Sounds/Interface/button.wav";
	soundFiles[9] = "../Audio/Sounds/Interface/pause.wav";
	//bosses
	soundFiles[10] = "../Audio/Sounds/Bosses/boss_clear.wav";
	soundFiles[11] = "../Audio/Sounds/Bosses/Bat/boss_bat_attack.wav";
	soundFiles[12] = "../Audio/Sounds/Bosses/Bat/boss_bat_hurt.wav";
	soundFiles[13] = "../Audio/Sounds/Bosses/Bat/boss_bat_death.wav";
	soundFiles[46] = "../Audio/Sounds/Bosses/Spider/boss_spider_spawn.wav";
	soundFiles[47] = "../Audio/Sounds/Bosses/Spider/boss_spider_attack.wav";
	soundFiles[48] = "../Audio/Sounds/Bosses/Spider/boss_spider_hurt.wav";
	soundFiles[49] = "../Audio/Sounds/Bosses/Spider/boss_spider_death.wav";
	soundFiles[50] = "../Audio/Sounds/Bosses/Spider/boss_spider_webshot.wav";
	soundFiles[52] = "../Audio/Sounds/Bosses/Ghost/boss_ghost_laugh.wav";
	soundFiles[53] = "../Audio/Sounds/Bosses/Ghost/boss_ghost_hurt.wav";
	soundFiles[54] = "../Audio/Sounds/Bosses/Ghost/boss_ghost_death.wav";
	soundFiles[62] = "../Audio/Sounds/Bosses/Grim/grim_intro.wav";
	soundFiles[63] = "../Audio/Sounds/Bosses/Grim/grim_transform.wav";
	soundFiles[64] = "../Audio/Sounds/Bosses/Grim/grim_hurt.wav";
	soundFiles[65] = "../Audio/Sounds/Bosses/Grim/grim_stage1_death.wav";
	soundFiles[66] = "../Audio/Sounds/Bosses/Grim/grim_stage2_death.wav";
	soundFiles[67] = "../Audio/Sounds/Bosses/Grim/grim_clap.wav";
	soundFiles[68] = "../Audio/Sounds/Bosses/Grim/grim_hand_hurt.wav";
	soundFiles[69] = "../Audio/Sounds/Bosses/Grim/grim_hand_stun.wav";
	soundFiles[70] = "../Audio/Sounds/Bosses/Grim/grim_player_death.wav";
	soundFiles[71] = "../Audio/Sounds/Bosses/Grim/grim_unlocked.wav";
	//enemies
	soundFiles[14] = "../Audio/Sounds/Enemies/enemy_slime_jump.wav";
	soundFiles[15] = "../Audio/Sounds/Enemies/enemy_slime_hurt.wav";
	soundFiles[16] = "../Audio/Sounds/Enemies/enemy_slime_death.wav";
	soundFiles[17] = "../Audio/Sounds/Enemies/enemy_flame_hurt.wav";
	soundFiles[18] = "../Audio/Sounds/Enemies/enemy_flame_death.wav";
	soundFiles[20] = "../Audio/Sounds/Enemies/enemy_ghost_hurt.wav";
	soundFiles[21] = "../Audio/Sounds/Enemies/enemy_ghost_death.wav";
	soundFiles[22] = "../Audio/Sounds/Enemies/enemy_ghost_moan.wav";
	soundFiles[23] = "../Audio/Sounds/Enemies/enemy_spider_jump.wav";
	soundFiles[24] = "../Audio/Sounds/Enemies/enemy_spider_hurt.wav";
	soundFiles[25] = "../Audio/Sounds/Enemies/enemy_spider_death.wav";
	soundFiles[19] = "../Audio/Sounds/Enemies/enemy_tome_spellcast.wav";
	soundFiles[55] = "../Audio/Sounds/Enemies/enemy_tome_hurt.wav";
	soundFiles[56] = "../Audio/Sounds/Enemies/enemy_tome_death.wav";
	soundFiles[57] = "../Audio/Sounds/Enemies/enemy_missile_death.wav";
	//items
	soundFiles[27] = "../Audio/Sounds/Items/item_hearth_piece.wav";
	soundFiles[28] = "../Audio/Sounds/Items/item_hearth_completed.wav";
	//ambient
	soundFiles[29] = "../Audio/Sounds/Ambient/ambient_rain.wav";
	soundFiles[30] = "../Audio/Sounds/Ambient/ambient_lightning.wav";
	soundFiles[31] = "../Audio/Sounds/Ambient/ambient_thunder.wav";
	soundFiles[32] = "../Audio/Sounds/Ambient/ambient_crow.wav";
	soundFiles[33] = "../Audio/Sounds/everyone.wav";
	soundFiles[34] = "../Audio/Sounds/Ambient/ambient_frostmourne_chant.wav";
	soundFiles[35] = "../Audio/Sounds/Ambient/ambient_mining.wav";
	soundFiles[36] = "../Audio/Sounds/Ambient/ambient_ironbeak_owl.wav";
	soundFiles[37] = "../Audio/Sounds/Ambient/ambient_bats.wav";
	soundFiles[38] = "../Audio/Sounds/Ambient/ambient_water_splash.wav";
	soundFiles[39] = "../Audio/Sounds/Ambient/Ghost/damned_scream.wav";
	soundFiles[40] = "../Audio/Sounds/Ambient/Ghost/ghost_moan.wav";
	soundFiles[41] = "../Audio/Sounds/Ambient/Ghost/damned_moan.wav";
	soundFiles[42] = "../Audio/Sounds/Ambient/Ghost/undead_moan.wav";
	soundFiles[43] = "../Audio/Sounds/Ambient/Ghost/undead_dissipate.wav";
	soundFiles[44] = "../Audio/Sounds/Ambient/rat_gnawing.wav";
	soundFiles[45] = "../Audio/Sounds/Ambient/rat_squeak.wav";
	soundFiles[58] = "../Audio/Sounds/Ambient/Ghost/creak_door.wav";
	soundFiles[59] = "../Audio/Sounds/Ambient/Ghost/creak_stair.wav";
	soundFiles[60] = "../Audio/Sounds/Ambient/Ghost/ghost_laugh.wav"; // same as ghost boss
	//...
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
		Debug::DebugOutput("%d bytes loaded\n", fread(buf, sizeof(char), waveFileHeader.dataSize, fp) );           //Read the sound data and display the 
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

void Audio::update(float deltaTime)
{
	for (unsigned int i = 0; i < musicSources.size(); i++) // check the state of all music sources
	{
		if (musicEnabled && audioEnabled)// music is enabled
		{
			if (musicSources[i].state == A_FADEIN) //new music track is fading in
			{
				musicSources[i].volume += FADEINTIME * deltaTime;
				if (musicSources[i].volume >= musicVolume)
				{
					musicSources[i].state = A_PLAYING;
					musicSources[i].volume = musicVolume;
				}
				alSourcef(musicSources[i].source, AL_GAIN, musicSources[i].volume * masterVolume);
			}

			else if (musicSources[i].state == A_FADEOUT) //old music track is fading out
			{
				musicSources[i].volume -= FADEOUTTIME * deltaTime;
				if (musicSources[i].volume <= 0.0f)
				{
					musicSources[i].volume = 0.0f;
					alSourceStop(musicSources[i].source);
				}
				alSourcef(musicSources[i].source, AL_GAIN, musicSources[i].volume * masterVolume);
			}
			else // audio re-enabled
			{
				alSourcef(musicSources[i].source, AL_GAIN, musicSources[i].volume * masterVolume);
			}
		}
		else // music is disabled
			alSourcef(musicSources[i].source, AL_GAIN, 0.0f);
	}

	// clean and remove music sources that's finished playing
	for (unsigned int i = 0; i < musicSources.size(); i++)
	{
		ALint state;
		alGetSourcei(musicSources[i].source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alDeleteSources(1, &musicSources[i].source);
			musicSources.erase(musicSources.begin() + i);
			break;
		}

	}

	// clean and remove sound sources that's finished playing
	for (unsigned int i = 0; i < soundSources.size(); i++)
	{
		ALint state;
		alGetSourcei(soundSources[i], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alDeleteSources(1, &soundSources[i]);
			soundSources.erase(soundSources.begin() + i);
			break;
		}
	}
	/* print # of buffers for debug purposes
	Debug::DebugOutput("Audio sources %i\n", soundSources.size());
	Debug::DebugOutput("sBuffers: %i, mBuffers: %i\n", soundSources.size(), musicSources.size());*/
}

void Audio::playMusic(int file)
{
	if (file < MUSIC_BUFFERS && file >= 0)
	{
		if (musicSources.size() == 0)// check if no music is playing yet
		{
			MusicStruct music;
			alGenSources(1, &music.source);

			ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
			ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

			alSourcei(music.source, AL_BUFFER, musicBuffer[file]);
			alSourcef(music.source, AL_PITCH, 1.0f);
			alSourcef(music.source, AL_GAIN, masterVolume * musicVolume);
			alSourcei(music.source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
			alSourcefv(music.source, AL_POSITION, SourcePos);
			alSourcefv(music.source, AL_VELOCITY, SourceVel);
			alSourcei(music.source, AL_LOOPING, AL_TRUE);

			music.volume = musicVolume;
			music.state = A_PLAYING;
			alSourcePlay(music.source);
			music.track = file;

			musicSources.push_back(music);
		}
		else if (musicSources.begin()->track != file) // check so that it's not the current track
		{
			MusicStruct music;
			alGenSources(1, &music.source);

			ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
			ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

			alSourcei(music.source, AL_BUFFER, musicBuffer[file]);
			alSourcef(music.source, AL_PITCH, 1.0f);
			alSourcef(music.source, AL_GAIN, masterVolume * musicVolume);
			alSourcei(music.source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
			alSourcefv(music.source, AL_POSITION, SourcePos);
			alSourcefv(music.source, AL_VELOCITY, SourceVel);
			alSourcei(music.source, AL_LOOPING, AL_TRUE);

			music.volume = musicVolume;
			music.state = A_PLAYING;
			alSourcePlay(music.source);
			music.track = file;

			alSourceStop(musicSources.begin()->source);

			musicSources.insert(musicSources.begin(), music); // add new music to the start of the vector
		}

	}
	else if (file != -2) // -2 keeps the current music
	{ //track is -1 stop music
		if (musicSources.size() > 0)
			alSourceStop(musicSources.begin()->source);
	}
}

void Audio::playMusicFade(int file, float deltaTime)
{
	if (file < MUSIC_BUFFERS && file >= 0)
	{
		if (musicSources.size() == 0)// check if no music is playing yet
		{
			MusicStruct music;
			alGenSources(1, &music.source);

			ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
			ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

			alSourcei(music.source, AL_BUFFER, musicBuffer[file]);
			alSourcef(music.source, AL_PITCH, 1.0f);
			alSourcef(music.source, AL_GAIN, 0.0f);
			alSourcei(music.source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
			alSourcefv(music.source, AL_POSITION, SourcePos);
			alSourcefv(music.source, AL_VELOCITY, SourceVel);
			alSourcei(music.source, AL_LOOPING, AL_TRUE);

			//fade in new track
			alSourcePlay(music.source);
			music.volume = 0.0f;
			music.state = A_FADEIN;
			music.track = file;

			//add new music source to the source list
			musicSources.push_back(music);
		}
		else if (musicSources.begin()->track != file) // check so that it's not the current track
		{
			MusicStruct music;
			alGenSources(1, &music.source);

			ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
			ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

			alSourcei(music.source, AL_BUFFER, musicBuffer[file]);
			alSourcef(music.source, AL_PITCH, 1.0f);
			alSourcef(music.source, AL_GAIN, 0.0f);
			alSourcei(music.source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
			alSourcefv(music.source, AL_POSITION, SourcePos);
			alSourcefv(music.source, AL_VELOCITY, SourceVel);
			alSourcei(music.source, AL_LOOPING, AL_TRUE);

			//fade in new track
			alSourcePlay(music.source);
			music.volume = 0.0f;
			music.state = A_FADEIN;
			music.track = file;

			//fade out old track
			musicSources.begin()->state = A_FADEOUT;

			//add new music source to the source list
			musicSources.insert(musicSources.begin(), music); // add new music to the start of the vector
		}

	}
	else if (file != -2) // -2 keeps the current music
	{ //track is -1 stop music
		if (musicSources.size() > 0)
			musicSources.begin()->state = A_FADEOUT;
	}
}

void Audio::playSound(int file, bool looping)
{
if (soundEnabled && audioEnabled) //sound is enabled
	if (file < SOUND_BUFFERS && soundSources.size() < SOUND_SOURCES) //check to see that there are available sound buffers
	{
		ALuint source;
		alGenSources(1, &source);

		ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcei(source, AL_BUFFER, soundBuffer[file]);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, masterVolume * soundVolume);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
		alSourcefv(source, AL_POSITION, SourcePos);
		alSourcefv(source, AL_VELOCITY, SourceVel);
		alSourcei(source, AL_LOOPING, looping);

		alSourcePlay(source);

		soundSources.push_back(source);
	}
}
void Audio::playSound(int file, bool looping, float pitch)
{
	if (soundEnabled && audioEnabled) //sound is enabled
	if (file < SOUND_BUFFERS && soundSources.size() < SOUND_SOURCES) //check to see that there are available sound buffers
	{
		ALuint source;
		alGenSources(1, &source);

		ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcei(source, AL_BUFFER, soundBuffer[file]);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_GAIN, masterVolume * soundVolume);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
		alSourcefv(source, AL_POSITION, SourcePos);
		alSourcefv(source, AL_VELOCITY, SourceVel);
		alSourcei(source, AL_LOOPING, looping);

		alSourcePlay(source);

		soundSources.push_back(source);
	}
}

void Audio::playSoundAtPos(int file, glm::vec3 pos, float distance, bool looping)
{
if (soundEnabled && audioEnabled) //sound is enabled
	if (file < SOUND_BUFFERS && soundSources.size() < SOUND_SOURCES) //check to see that there are available sound buffers
	{
		ALuint source;
		alGenSources(1, &source);

		ALfloat SourcePos[] = { pos.x, pos.y, pos.z };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcei(source, AL_BUFFER, soundBuffer[file]);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, masterVolume * soundVolume);
		alSourcei(source, AL_REFERENCE_DISTANCE, distance);
		alSourcefv(source, AL_POSITION, SourcePos);
		alSourcefv(source, AL_VELOCITY, SourceVel);
		alSourcei(source, AL_LOOPING, looping);

		alSourcePlay(source);

		soundSources.push_back(source);
	}
}

void Audio::playSoundAtPos(int file, glm::vec3 pos, float distance, bool looping, float pitch)
{
	if (soundEnabled && audioEnabled) //sound is enabled
	if (file < SOUND_BUFFERS && soundSources.size() < SOUND_SOURCES) //check to see that there are available sound buffers
	{
		ALuint source;
		alGenSources(1, &source);

		ALfloat SourcePos[] = { pos.x, pos.y, pos.z };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcei(source, AL_BUFFER, soundBuffer[file]);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_GAIN, masterVolume * soundVolume);
		alSourcei(source, AL_REFERENCE_DISTANCE, distance);
		alSourcefv(source, AL_POSITION, SourcePos);
		alSourcefv(source, AL_VELOCITY, SourceVel);
		alSourcei(source, AL_LOOPING, looping);

		alSourcePlay(source);

		soundSources.push_back(source);
	}
}

ALuint Audio::playSoundSP(int file, bool looping)
{
	if (soundEnabled && audioEnabled) //sound is enabled
	if (file < SOUND_BUFFERS && soundSources.size() < SOUND_SOURCES) //check to see that there are available sound buffers
	{
		ALuint source;
		alGenSources(1, &source);

		ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

		alSourcei(source, AL_BUFFER, soundBuffer[file]);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, masterVolume * soundVolume);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE); // 2D sound
		alSourcefv(source, AL_POSITION, SourcePos);
		alSourcefv(source, AL_VELOCITY, SourceVel);
		alSourcei(source, AL_LOOPING, looping);

		alSourcePlay(source);

		soundSources.push_back(source);

		return source;
	}

	return NULL;
}

ALuint Audio::playSoundAtPosSP(int file, glm::vec3 pos, float distance, bool looping)
{
	if (soundEnabled && audioEnabled) //sound is enabled
		if (file < SOUND_BUFFERS && soundSources.size() < SOUND_SOURCES) //check to see that there are available sound buffers
		{
			ALuint source;
			alGenSources(1, &source);

			ALfloat SourcePos[] = { pos.x, pos.y, pos.z };
			ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

			alSourcei(source, AL_BUFFER, soundBuffer[file]);
			alSourcef(source, AL_PITCH, 1.0f);
			alSourcef(source, AL_GAIN, masterVolume * soundVolume);
			alSourcei(source, AL_REFERENCE_DISTANCE, distance);
			alSourcefv(source, AL_POSITION, SourcePos);
			alSourcefv(source, AL_VELOCITY, SourceVel);
			alSourcei(source, AL_LOOPING, looping);

			alSourcePlay(source);

			soundSources.push_back(source);

			return source;
		}

	return NULL;
}

void Audio::updateListener(glm::vec3 pos)
{
	// convert from float to ALfloat
	listenerPos[0] = pos.x;
	listenerPos[1] = pos.y;
	listenerPos[2] = pos.z;

	// update pos
	alListenerfv(AL_POSITION, listenerPos);
}

ALfloat* Audio::getListenerPos()
{
	return listenerPos;
}

void Audio::shutdown()
{
	// music
	musicSources.clear();
	alDeleteBuffers(MUSIC_BUFFERS, musicBuffer);

	// sounds
	soundSources.clear();
	alDeleteBuffers(SOUND_BUFFERS, soundBuffer);

	// device and devcon
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

int Audio::endWithError(char* msg, int error)
{
	// display error message in console
	cout << msg << "\n";
	system("PAUSE");
	return error;
}
