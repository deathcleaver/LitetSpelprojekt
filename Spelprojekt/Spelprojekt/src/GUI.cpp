#include "GUI.h"

void GUI::init(UserInput* in, Player* player, ContentManager* content, bool grayContinue)
{
	this->player = player;
	this->in = in;
	this->content = content;
	capacity = 30;
	items = new ScreenItem*[capacity];
	for (int n = 0; n < capacity; n++)
	{
		items[n] = 0;
	}
	size = 0;
	this->grayContinue = grayContinue;
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
	if (init)
	{
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
		size = 6;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();

		//logo
		items[0]->init(0, 0);
		items[0]->MoveAutoSize(0, 0.6, content);

		//new game button
		items[1]->init(1, 2, true, 1);
		items[1]->MoveAutoSize(0, 0.1, content);

		if (grayContinue)
		{
			//continue button
			items[2]->init(3, 4, true, 4, false);
			items[2]->MoveAutoSize(0, -0.15, content);
			//grayed out
			items[3]->init(5, 5, false, 0);
			items[3]->MoveAutoSize(0, -0.15, content);
		}
		else
		{
			//continue button
			items[2]->init(3, 4, true, 4);
			items[2]->MoveAutoSize(0, -0.15, content);
			//NOT grayed out
			items[3]->init(5, 5, false, 0, false);
			items[3]->MoveAutoSize(0, -0.15, content);
		}

		//map maker button
		items[4]->init(6, 7, true, 2);
		items[4]->MoveAutoSize(0, -0.4, content);

		//credits button
		items[5]->init(8, 9, true, 0);
		items[5]->MoveAutoSize(0, -0.6, content);
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
		size = 11;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();
		//Hud
		items[0]->init(50, 50);
		items[0]->MoveAutoSize(0, -0.8, content);

		//World Switch
		items[1]->init(51, 52, true, 100);
		items[1]->MoveAutoSize(-0.97, -0.7, content);
		items[1]->initSwitch();

		//Rekt Switch
		items[2]->init(51, 52, true, 101);
		items[2]->MoveAutoSize(-0.97, -0.789, content);
		items[2]->initSwitch();

		//Light Switch
		items[3]->init(51, 52, true, 102);
		items[3]->MoveAutoSize(-0.97, -0.878, content);
		items[3]->initSwitch();

		//Enemy Switch
		items[4]->init(51, 52, true, 103);
		items[4]->MoveAutoSize(-0.97, -0.967, content);
		items[4]->initSwitch();

		//Back Switch
		items[5]->init(51, 52, true, 104);
		items[5]->MoveAutoSize(-0.73, -0.7, content);
		items[5]->initSwitch();

		//Shrine Switch
		items[6]->init(51, 52, true, 105);
		items[6]->MoveAutoSize(-0.73, -0.789, content);
		items[6]->initSwitch();

		//Boss Switch
		items[7]->init(51, 52, true, 106);
		items[7]->MoveAutoSize(-0.73, -0.878, content);
		items[7]->initSwitch();

		//Audio Switch
		items[8]->init(51, 52, true, 107);
		items[8]->MoveAutoSize(-0.73, -0.967, content);
		items[8]->initSwitch();

		//Place Switch
		items[9]->init(51, 52, true, 110);
		items[9]->MoveAutoSize(0.04, -0.785, content);
		items[9]->initSwitch();

		//Change Switch
		items[10]->init(51, 52, true, 111);
		items[10]->MoveAutoSize(0.04, -0.875, content);
		items[10]->initSwitch();
	}
}

void GUI::PAUSE(bool init)
{
	if (init)
	{
		size = 2;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();
		// paused text
		items[0]->init(20, 20);
		items[0]->MoveAutoSize(0, 0.2, content);
		// exit text
		items[1]->init(21, 22, true, 3);
		items[1]->MoveAutoSize(0, -0.1, content);
	}
}

int GUI::keyUpdate()
{
	int ret = 0;
	float x, y;
	bool rmb;
	bool lmb;

	in->getMouseState(&x, &y, &rmb, &lmb);
	MouseToScreenSpace(&x, &y);
	for (int n = 0; n < size; n++)
	{
		//temp click
		if (items[n]->visable())
		{
			ret = items[n]->update(x, y, lmb);
			if (ret != 0)
			{
				if (items[0]->hoverContentIndex > 49) // in editor
				{
					//if its a editor mode switch
					if (n > 0 && n < 9)
					{
						modeSwitchesFalse();
						items[n]->setActive(true);
					}
					//if its a edit state switch
					else if (n == 9)
						items[10]->setActive(false);
					else if (n == 10)
						items[9]->setActive(false);

				}
				return ret;
			}
		}
	}
	return ret;
}

int GUI::readSize() const
{
	return size;
}

ScreenItem** GUI::getItems() const
{
	return items;
}

void GUI::MouseToScreenSpace(float* x, float* y)
{
	*x = (*x / (SCREENWIDTH * 0.5)) -1;
	*y = ((*y / (SCREENHEIGHT * 0.5)) -1) * -1;
}

void GUI::modeSwitchesFalse()
{
	for (int n = 1; n < 9; n++)
	{
		items[n]->setActive(false);
	}
}

void GUI::fixEditorSwitches(bool place, bool change)
{
	// 9 place
	items[9]->setActive(place);
	//10 change
	items[10]->setActive(change);
}