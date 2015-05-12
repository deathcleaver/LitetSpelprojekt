#ifndef GUI_H
#define GUI_H

#include"GameConfig.h"
#include "screenItem.h"
#include "UserInput.h"
#include "game\player.h"
#include "engine\ContentManager.h"

class GUI
{
private:
	UserInput* in;
	Player* player;
	ContentManager* content;
	int current = 0;
	int last = -1;
	ScreenItem** items = 0;
	
	//intro sync
	int introCounter = 0;
	int introTime = 220;
	int introState = 0;

	//hud display
	int playerHP = 0;
	int playerPickups = 0;
	int playerRune = 0;
	int playerShield = 0;

	//boss defeated, progress display
	int progressID;
	int progressCounter = 0;
	int progressTime = 210;
	bool showBossProgress = false;
	bool re_setup = false;

	int size = 0;
	int capacity = 0;
	void destroyy();
	void MENU(bool init);
	void PLAY(bool init);
	void playerhud(bool init);
	void newupgradeInit();
	void INTRO(bool init);
	void EDIT(bool init);
	void PAUSE(bool init);
	void SETTINGS(bool init);
	void CREDITS(bool init);
	void bossprogressInit();
	int keyUpdate();
	float SCREENWIDTH = (float)GameConfig::get().configResX;
	float SCREENHEIGHT = (float)GameConfig::get().configResY;
	void modeSwitchesFalse();
	bool grayContinue = true;
public:
	GUI(){};
	~GUI();
	void init(UserInput* in, Player* player, ContentManager* content, bool grayContinue = true);
	void setIntroState(int i){ introState = i; };
	int update(int state);
	int GUI::readSize() const;
	int GUI::bindIndex(int index) const;
	ScreenItem** getItems() const;
	void MouseToScreenSpace(float* x, float* y);
	void fixEditorSwitches(bool place, bool change);
	void showNewUpgrade(int);

	void newPlayerRef(Player* p)
	{
		player = p;
	}

	void setGrayContinue(bool isGray);
};

#endif