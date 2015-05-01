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
		case(1) : // play
			//moved out of the switch case
			break;
		case(2) : //intro
			//moved out of the swtich case
			break;
		case(3) :
			EDIT(init);
			break;
		case(4) :
			PAUSE(init);
			break;
		case(5) :
			SETTINGS(init);
			break;
		}
	}
	if (state == 1) //if play
		PLAY(init);
	
	if (state == 2) // if intro
	{
		INTRO(init);
		if (introState == 6)
			return 1; //playmode
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
		items[1]->init(1, 2, true, 5); //puts the game in intro state
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

		//settings button

		//credits button
		items[5]->init(8, 9, true, 0);
		items[5]->MoveAutoSize(0, -0.6, content);
	}
}

void GUI::PLAY(bool init)
{
	int hp;
	int pickups;
	bool refresh = false;

	player->playerGuiInfo(&hp, &pickups);
	if (hp != playerHP || pickups != playerPickups)
	{
		playerHP = hp;
		playerPickups = pickups;
		refresh = true;
	}

	if (init || refresh)
	{
		destroyy();

		size = 1 + playerHP;
		bool extra = false;
		if (playerPickups > 0)
		{
			size++;
			extra = true;
		}

		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();

		items[0]->init(25, 25, false);
		items[0]->MoveAutoSize(-0.57, -0.78, content);

		if (extra)
			size--;

		for (int n = 1; n < size ; n++)
		{
			items[n]->init(28, 28, false);
			items[n]->MoveAutoSize(-0.68 + 0.12 * (n-1), -0.8, content);
		}

		if (extra)
		{
			size++;

			if (playerPickups == 1)
			{
				items[size - 1]->init(26, 26, false);
				items[size - 1]->MoveAutoSize(-0.68 + 0.12 * (size - 2), -0.8, content);
			}
			else if (playerPickups == 2)
			{
				items[size - 1]->init(27, 27, false);
				items[size - 1]->MoveAutoSize(-0.68 + 0.12 * (size - 2), -0.8, content);
			}
		}
	}
}

void GUI::INTRO(bool init)
{
	if (init) //setup
	{
		introState = 0;
		introCounter = 0;
	}

	introCounter++;
	if (introCounter > introTime)
	{
		introCounter = 0;
		introState++;
		init = true;
		destroyy();
	}
	if (init)
	{
		if (introState < 6) //intro state 6 will return into playmode outside of this function
		{
			size = 1;
			items[0] = new ScreenItem();
			items[0]->init(60 + introState, 60 + introState, true, 1);
			items[0]->MoveAutoSize(0, 0, content);
		}
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

void GUI::SETTINGS(bool init)
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