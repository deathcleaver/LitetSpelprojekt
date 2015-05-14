#include "GUI.h"
#include "GameConfig.h"

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
		case(6) :
			SETTINGS(init);
			break;
			// settings
		case(7) : // toggle audio
			break;
		case(8) : // toggle music
			break;
		case(9) : // toggle sound	
			break;
		case(10) : // toggle resolution
			break;
		case(11): // credits gui (empty)
			CREDITS(init);
			break;
		case(12) : // difficulty gui
			DIFFICULTY(init);
			break;
		case(16) : // toggle firstperson
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
		size = 7;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();

		//logo
		items[0]->init(0, 0);
		items[0]->MoveAutoSize(0, 0.6f, content);

		//new game button
		items[1]->init(1, 2, true, 12); //puts the game in intro state
		items[1]->MoveAutoSize(0, 0.1f, content);

		if (grayContinue)
		{
			//continue button
			items[2]->init(3, 4, true, 4, false);
			items[2]->MoveAutoSize(0, -0.15f, content);
			//grayed out
			items[3]->init(5, 5, false, 0);
			items[3]->MoveAutoSize(0, -0.15f, content);
		}
		else
		{
			//continue button
			items[2]->init(3, 4, true, 4);
			items[2]->MoveAutoSize(0, -0.15f, content);
			//NOT grayed out
			items[3]->init(5, 5, false, 0, false);
			items[3]->MoveAutoSize(0, -0.15f, content);
		}

		//map maker button
		items[4]->init(6, 7, true, 2);
		items[4]->MoveAutoSize(0, -0.4f, content);

		//credits button
		items[5]->init(8, 9, true, 11);
		items[5]->MoveAutoSize(0, -0.6f, content);
		
		//settings button
		items[6]->init(10, 11, true, 6);
		items[6]->MoveAutoSize(0, -0.8f, content);
	}
}

void GUI::PLAY(bool init)
{
	if (showBossProgress)
		newupgradeInit();

	if (progressCounter < 0) // update playerhud normaly
		playerhud(init);
	else
		progressCounter--;
	
}

void GUI::playerhud(bool init)
{
	int hp;
	int pickups;
	int rune;
	int shield;
	bool refresh = false;

	if (re_setup)
		init = true;

	player->playerGuiInfo(&hp, &pickups, &rune, &shield);
	if (hp != playerHP || pickups != playerPickups ||
		rune != playerRune || shield != playerShield)
	{
		playerHP = hp;
		playerPickups = pickups;
		playerRune = rune;
		playerShield = shield;
		
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
		items[0]->MoveAutoSize(-0.57f, -0.78f, content);

		if (extra)
			size--;

		for (int n = 1; n < size; n++)
		{
			items[n]->init(28, 28, false);
			items[n]->MoveAutoSize(-0.68f + 0.12f * (n - 1.0f), -0.8f, content);
		}

		if (extra)
		{
			size++;

			if (playerPickups == 1)
			{
				items[size - 1]->init(26, 26, false);
				items[size - 1]->MoveAutoSize(-0.68f + 0.12f * (size - 2.0f), -0.8f, content);
			}
			else if (playerPickups == 2)
			{
				items[size - 1]->init(27, 27, false);
				items[size - 1]->MoveAutoSize(-0.68f + 0.12f * (size - 2.0f), -0.8f, content);
			}
		}

		if (rune == 1)
		{
			items[size] = new ScreenItem();
			items[size]->init(32, 32, false);
			items[size]->MoveAutoSize(-0.68f + 0.12f * (size - 1.0f), -0.8f, content);
			size++;
		}
		if (rune == 2)
		{
			items[size] = new ScreenItem();
			items[size]->init(33, 33, false);
			items[size]->MoveAutoSize(-0.68f + 0.12f * (size - 1.0f), -0.8f, content);
			size++;
		}
		if (shield > 0)
		{
			
			items[size] = new ScreenItem();
			items[size]->init(34, 34, false);
			items[size]->MoveAutoSize(-0.68f + 0.12f * (size - 1.0f), -0.8f, content);
			size++;
			if (shield > 1)
			{
				items[size] = new ScreenItem();
				items[size]->init(34, 34, false);
				items[size]->MoveAutoSize(-0.68f + 0.12f * (size - 1.0f), -0.8f, content);
				size++;
			}
		}
	}
}

void GUI::newupgradeInit()
{
	re_setup = true;
	progressCounter = progressTime;
	showBossProgress = false;

	destroyy();

	if(progressID < 4) //dont show progress on boss conversation
		bossprogressInit();

	if (progressID > 0)
	{
		items[size] = new ScreenItem();

		if (progressID == 1) //duble jump
			items[size]->init(29, 29);
		else if (progressID == 2) // web climp
			items[size]->init(30, 30);
		else if (progressID == 3) // mirror walk
			items[size]->init(31, 31);

		else if (progressID == 4) // boss intro 
			items[size]->init(35, 35);
		else if (progressID == 5) // boss final form
			items[size]->init(36, 36);
		else if (progressID == 6) // boss scream
			items[size]->init(37, 37);

		items[size]->MoveAutoSize(0, -0.3f, content);

		size++;
	}
	
}

void GUI::showNewUpgrade(int id)
{
	showBossProgress = true;
	progressID = id;
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
		items[0]->MoveAutoSize(0, -0.8f, content);

		//World Switch
		items[1]->init(51, 52, true, 100);
		items[1]->MoveAutoSize(-0.97f, -0.7f, content);
		items[1]->initSwitch();

		//Rekt Switch
		items[2]->init(51, 52, true, 101);
		items[2]->MoveAutoSize(-0.97f, -0.789f, content);
		items[2]->initSwitch();

		//Light Switch
		items[3]->init(51, 52, true, 102);
		items[3]->MoveAutoSize(-0.97f, -0.878f, content);
		items[3]->initSwitch();

		//Enemy Switch
		items[4]->init(51, 52, true, 103);
		items[4]->MoveAutoSize(-0.97f, -0.967f, content);
		items[4]->initSwitch();

		//Back Switch
		items[5]->init(51, 52, true, 104);
		items[5]->MoveAutoSize(-0.73f, -0.7f, content);
		items[5]->initSwitch();

		//Shrine Switch
		items[6]->init(51, 52, true, 105);
		items[6]->MoveAutoSize(-0.73f, -0.789f, content);
		items[6]->initSwitch();

		//Boss Switch
		items[7]->init(51, 52, true, 106);
		items[7]->MoveAutoSize(-0.73f, -0.878f, content);
		items[7]->initSwitch();

		//Audio Switch
		items[8]->init(51, 52, true, 107);
		items[8]->MoveAutoSize(-0.73f, -0.967f, content);
		items[8]->initSwitch();

		//Place Switch
		items[9]->init(51, 52, true, 110);
		items[9]->MoveAutoSize(0.04f, -0.785f, content);
		items[9]->initSwitch();

		//Change Switch
		items[10]->init(51, 52, true, 111);
		items[10]->MoveAutoSize(0.04f, -0.875f, content);
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
		items[0]->MoveAutoSize(0, 0.2f, content);
		// exit text
		items[1]->init(21, 22, true, 3);
		items[1]->MoveAutoSize(0, -0.1f, content);

		//show boss progress
		bossprogressInit();
	}
}

void GUI::DIFFICULTY(bool init)
{
	if (init)
	{
		size = 4;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();
		
			items[0]->init(81, 81); // title
			items[0]->MoveAutoSize(0, 0.6f, content);

			items[1]->init(75, 76, true, 13);
			items[1]->MoveAutoSize(0, 0.2f, content);
			
			items[2]->init(77, 78, true, 14);
			items[2]->MoveAutoSize(0, -0.2f, content);
			
			items[3]->init(79, 80, true, 15);
			items[3]->MoveAutoSize(0, -0.6f, content);
	}
}

void GUI::SETTINGS(bool init)
{
	if (init)
	{
		size = 5;
		for (int n = 0; n < size; n++)
			items[n] = new ScreenItem();

		// audio enabled
		if (Audio::getAudio().getAudioEnabled() == true)
		{
			items[0]->init(12, 12, true, 7);
			items[0]->MoveAutoSize(0, 0.6f, content);
			items[0]->initSwitch();
		}
		else
		{
			items[0]->init(13, 13, true, 7);
			items[0]->MoveAutoSize(0, 0.6f, content);
			items[0]->initSwitch();
		}
		
		// music enabled
		if (Audio::getAudio().getMusicEnabled() == true)
		{
			items[1]->init(14, 14, true, 8);
			items[1]->MoveAutoSize(0, 0.2f, content);
			items[1]->initSwitch();
		}
		else
		{
			items[1]->init(15, 15, true, 8);
			items[1]->MoveAutoSize(0, 0.2f, content);
			items[1]->initSwitch();
		}
		// sound enabled
		if (Audio::getAudio().getSoundEnabled() == true)
		{
			items[2]->init(16, 16, true, 9);
			items[2]->MoveAutoSize(0, -0.2f, content);
			items[2]->initSwitch();
		}
		else
		{
			items[2]->init(17, 17, true, 9);
			items[2]->MoveAutoSize(0, -0.2f, content);
			items[2]->initSwitch();
		}
		// toggle fullscreen
		if (GameConfig::get().configFullscreen == true)
		{
			items[3]->init(18, 18, true, 10);
			items[3]->MoveAutoSize(0, -1.0, content);
			items[3]->initSwitch();
		}
		else
		{
			items[3]->init(19, 19, true, 10);
			items[3]->MoveAutoSize(0, -1.0, content);
			items[3]->initSwitch();
		}
		// toggle fps
		if (GameConfig::get().configFullscreen == true)
		{
			items[4]->init(82, 82, true, 16);
			items[4]->MoveAutoSize(0, -0.6, content);
			items[4]->initSwitch();
		}
		else
		{
			items[4]->init(83, 83, true, 16);
			items[4]->MoveAutoSize(0, -0.6, content);
			items[4]->initSwitch();
		}

	}
}

void GUI::CREDITS(bool init)
{
	if (init)
		size = 0;
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
				if (items[0]->hoverContentIndex > 49 && items[0]->hoverContentIndex < 59) // in editor
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
	*x = (*x / (SCREENWIDTH * 0.5f)) -1.0f;
	*y = ((*y / (SCREENHEIGHT * 0.5f)) -1.0f) * -1.0f;
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

void GUI::bossprogressInit()
{
	int counter = 0;
	
	if (player->getProgress().batboss)
		counter++;
	if (player->getProgress().spiderboss)
		counter++;
	if (player->getProgress().ghostboss)
		counter++;

	int oldsize = size;
	size = size + 1 + counter;

	for (int n = oldsize; n < size; n++)
		items[n] = new ScreenItem();
	
	// boss back
	items[oldsize]->init(23, 23);
	items[oldsize]->MoveAutoSize(0, 0.7f, content);
	//defeated cross
	int increment = 1;
	if (player->getProgress().batboss)
	{
		items[oldsize + increment]->init(24, 24);
		items[oldsize + increment]->MoveAutoSize(-0.33f, 0.70f, content);
		increment++;
	}
	if (player->getProgress().spiderboss)
	{
		items[oldsize + increment]->init(24, 24);
		items[oldsize + increment]->MoveAutoSize(-0.05f, 0.71f, content);
		increment++;
	}
	if (player->getProgress().ghostboss)
	{
		items[oldsize + increment]->init(24, 24);
		items[oldsize + increment]->MoveAutoSize(0.24f, 0.71f, content);
		increment++;
	}
}

void GUI::setGrayContinue(bool isGray)
{
	grayContinue = isGray;
	destroyy();
	last = 0;
	MENU(true);
}