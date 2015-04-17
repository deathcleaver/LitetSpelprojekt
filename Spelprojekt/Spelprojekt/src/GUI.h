#ifndef GUI_H
#define GUI_H

#include "screenItem.h"

enum menuState
{
	gMENU,
	gPLAY,
	gINTRO,
	gEDIT,
	gPAUSE,
};

class GUI
{
private:
	menuState current = gPAUSE;
	menuState last = gPAUSE;
	ScreenItem* items = 0;
	int size = 0;
	int capacity = 0;
	void destroy();
	void MENU();
	void PLAY();
	void INTRO();
	void EDIT();
	void PAUSE();
public:
	GUI();
	~GUI();
	void init();
	void update(int state);
	
};

#endif