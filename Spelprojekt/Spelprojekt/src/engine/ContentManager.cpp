#include "ContentManager.h"

ContentManager::~ContentManager()
{
	
	if (guiObjs)
	{
		for (size_t i = 0; i < guiCount; i++)
		{
			delete guiObjs[i];
		}
		delete[] guiObjs;
	}
	
	delete objectManager;
}

void ContentManager::init()
{
	loadObjManager();
	//load all gui textures
	loadGUIobjs();

}

void ContentManager::update()
{
	objectManager->update();
}

void ContentManager::loadObjManager()
{
	if (objectManager)
		throw;
	objectManager = new ObjectManager();
	objectManager->init();
}

void ContentManager::loadGUIobjs()
{
	if (guiObjs)
		throw;

	guiCount = 75;
	guiObjs = new Object*[guiCount]();

	//main menu reserved 0 - 19
	guiObjs[0] = new Object("src/meshes/BaseBlit.v", "src/textures/gui/main/logo.bmp");
	guiObjs[1] = new Object("", "src/textures/gui/main/newgame.bmp", guiObjs[0], true);
	guiObjs[2] = new Object("", "src/textures/gui/main/newgameH.bmp", guiObjs[0], true);
	guiObjs[3] = new Object("", "src/textures/gui/main/continue.bmp", guiObjs[0], true);
	guiObjs[4] = new Object("", "src/textures/gui/main/continueH.bmp", guiObjs[0], true);
	guiObjs[5] = new Object("", "src/textures/gui/main/continueG.bmp", guiObjs[0], true);
	guiObjs[6] = new Object("", "src/textures/gui/main/mapmaker.bmp", guiObjs[0], true);
	guiObjs[7] = new Object("", "src/textures/gui/main/mapmakerH.bmp", guiObjs[0], true);
	guiObjs[8] = new Object("", "src/textures/gui/main/credits.bmp", guiObjs[0], true);
	guiObjs[9] = new Object("", "src/textures/gui/main/creditsH.bmp", guiObjs[0], true);
	for (int n = 10; n < 20; n++)
		guiObjs[n] = 0;

	//paused reserved 20 - 24
	guiObjs[20] = new Object("", "src/textures/gui/pause/paused.bmp", guiObjs[0], true);
	guiObjs[21] = new Object("", "src/textures/gui/pause/exit.bmp", guiObjs[0], true);
	guiObjs[22] = new Object("", "src/textures/gui/pause/exitH.bmp", guiObjs[0], true);
	for (int n = 23; n < 25; n++)
		guiObjs[n] = 0;

	// game play reserved 25 - 49
	for (int n = 25; n < 50; n++)
		guiObjs[n] = 0;

	// edit reserved 50 - 74
	guiObjs[50] = new Object("", "src/textures/gui/edit/editHud.bmp", guiObjs[0], true);
	guiObjs[51] = new Object("", "src/textures/gui/edit/switch.bmp", guiObjs[0], true);
	guiObjs[52] = new Object("", "src/textures/gui/edit/switchON.bmp", guiObjs[0], true);

	for (int n = 53; n < 75; n++)
		guiObjs[n] = 0;
}

void ContentManager::bindRekt() const
{
	objectManager->bindRekt();
}

int ContentManager::bindPlayer() const
{
	//player->updateWeight();
	/*player->bind();
	return player->getFaces();*/
	return objectManager->bindPlayer();
}

int::ContentManager::bind(int type, int id) const
{
	return objectManager->bind(type, id);
}

void ContentManager::bindGUItex(int id) const
{
	guiObjs[id]->bindTexOnly();
}

void ContentManager::bindGUIvert() const
{
	guiObjs[0]->bindVertOnly();
}

Object** ContentManager::gui()
{
	return guiObjs;
}

void ContentManager::setPlayerState(std::string state)
{
	objectManager->setPlayerState(state);
}

int ContentManager::nrOfWorldItems() const
{
	return objectManager->nrOfWorldItems();
}
int ContentManager::nrOfMonsterItems() const
{
	return objectManager->nrOfMonsterItems();
}