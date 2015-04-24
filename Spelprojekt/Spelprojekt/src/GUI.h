#ifndef GUI_H
#define GUI_H

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
	int size = 0;
	int capacity = 0;
	void destroyy();
	void MENU(bool init);
	void PLAY(bool init);
	void INTRO(bool init);
	void EDIT(bool init);
	void PAUSE(bool init);
	int keyUpdate();
	float SCREENWIDTH = 1080;
	float SCREENHEIGHT = 720;
	
	bool grayContinue = true;
public:
	GUI(){};
	~GUI();
	void init(UserInput* in, Player* player, ContentManager* content, bool grayContinue = true);
	int update(int state);
	int GUI::readSize() const;
	int GUI::bindIndex(int index) const;
	ScreenItem** getItems() const;
	void MouseToScreenSpace(float* x, float* y);
};

#endif