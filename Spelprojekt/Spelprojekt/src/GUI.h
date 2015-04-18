#ifndef GUI_H
#define GUI_H

#include "screenItem.h"
#include "UserInput.h"
#include "game\player.h"


class GUI
{
private:
	UserInput* in;
	Player* player;
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
public:
	GUI(){};
	~GUI();
	void init(UserInput* in, Player* player);
	int update(int state);
	int GUI::readSize() const;
	int GUI::bindIndex(int index) const;
	ScreenItem** getItems() const;
};

#endif