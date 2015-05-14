#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <vector>

class GameConfig
{
public:
	static GameConfig& get()
	{
		static GameConfig singleton;
		return singleton;
	}


public:
	enum Difficulty
	{
		Casual = 0,
		Hardcore = 1,
		DmonInHell = 2
	};

	int configDifficulty;
	bool configRenderGlow;
	bool configFirstperson;
	bool configFullscreen;
	int configResX, configResY;
};

#endif