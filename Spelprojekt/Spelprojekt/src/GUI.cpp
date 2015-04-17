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
	case(0) :
		current = gMENU;
		MENU();
		break;
	case(0) :
		current = gMENU;
		MENU();
		break;
	}
}