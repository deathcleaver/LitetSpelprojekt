#include "GUI.h"

void GUI::init(UserInput* in, Player* player)
{
	this->player = player;
	this->in = in;
	capacity = 30;
	items = new ScreenItem*[capacity];
	for (int n = 0; n < capacity; n++)
	{
		items[n] = 0;
	}
	size = 0;

}

GUI::~GUI()
{
	for (int n = 0; n < size; n++)
	{
		delete items[n];
	}
	delete[] items;
}

int GUI::update(int state)
{
	bool init = false;
	if (state != last)
	{
		init = true;
		destroyy();
	}
	switch (state)
	{
	case(0) :
		MENU(init);
		break;
	case(1) :
		PLAY(init);
		break;
	case(2) :
		INTRO(init);
		break;
	case(3) :
		EDIT(init);
		break;
	case(4) :
		PAUSE(init);
		break;
	}
	last = state;
	return keyUpdate();
}

void GUI::destroyy()
{
	for (int n = 0; n < size; n++)
	{
		delete items[n];
		items[n] = 0;
	}
	size = 0;
}

void GUI::MENU(bool init)
{
	if (init)
	{

	}
}

void GUI::PLAY(bool init)
{
	if (init)
	{

	}
}

void GUI::INTRO(bool init)
{
	if (init)
	{

	}
}

void GUI::EDIT(bool init)
{
	if (init)
	{

	}
}

void GUI::PAUSE(bool init)
{
	if (init)
	{
		size = 1;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();
		
		items[0]->init(0, 0);
	}
}

int GUI::keyUpdate()
{
	int ret = 0;
	for (int n = 0; n < size; n++)
	{
		//temp click
		ret = items[n]->update(0, 0, false);
		if (ret != 0)
			return ret;
	}
}

int GUI::readSize() const
{
	return size;
}

ScreenItem** GUI::getItems() const
{
	return items;
}