#include "GUI.h"

void GUI::init()
{
	capacity = 30;
	items = new ScreenItem[capacity];
	size = 0;

}

void GUI::update(int state)
{
	switch (state)
	{
	case(0) :
		current = gMENU;
		MENU();
		break;
	case(1) :
		current = gMENU;
		MENU();
		break;
	case(2) :
		current = gMENU;
		MENU();
		break;
	}
}

void GUI::destroy()
{
}

void GUI::MENU()
{
}

void GUI::PLAY()
{
}

void GUI::INTRO()
{
}

void GUI::EDIT()
{
}

void GUI::PAUSE()
{
}