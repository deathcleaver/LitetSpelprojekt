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

void ContentManager::update(UpdateAnimCheck* animCheck)
{
	objectManager->update(animCheck);
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
	guiObjs[10] = new Object("", "src/textures/gui/main/settings.bmp", guiObjs[0], true);
	guiObjs[11] = new Object("", "src/textures/gui/main/settingsH.bmp", guiObjs[0], true);
	guiObjs[12] = new Object("", "src/textures/gui/main/settings/audio.bmp", guiObjs[0], true);
	guiObjs[13] = new Object("", "src/textures/gui/main/settings/audioH.bmp", guiObjs[0], true);
	guiObjs[14] = new Object("", "src/textures/gui/main/settings/graphics.bmp", guiObjs[0], true);
	guiObjs[15] = new Object("", "src/textures/gui/main/settings/graphicsH.bmp", guiObjs[0], true);
	guiObjs[16] = new Object("", "src/textures/gui/main/settings/checkbox_no.bmp", guiObjs[0], true);
	guiObjs[17] = new Object("", "src/textures/gui/main/settings/checkbox_yes.bmp", guiObjs[0], true);
	for (int n = 18; n < 20; n++)
		guiObjs[n] = 0;

	//paused, exit to menu
	guiObjs[20] = new Object("", "src/textures/gui/pause/paused.bmp", guiObjs[0], true);
	guiObjs[21] = new Object("", "src/textures/gui/pause/exit.bmp", guiObjs[0], true);
	guiObjs[22] = new Object("", "src/textures/gui/pause/exitH.bmp", guiObjs[0], true);

	//boss progress
	guiObjs[23] = new Object("", "src/textures/gui/progress/bosses.bmp", guiObjs[0], true);
	guiObjs[24] = new Object("", "src/textures/gui/progress/defeated.bmp", guiObjs[0], true);

	// game play reserved 25 - 49
	//hud
	guiObjs[25] = new Object("", "src/textures/gui/play/skeletonHUD.bmp", guiObjs[0], true);
	guiObjs[26] = new Object("", "src/textures/gui/play/heart1.bmp", guiObjs[0], true);
	guiObjs[27] = new Object("", "src/textures/gui/play/heart2.bmp", guiObjs[0], true);
	guiObjs[28] = new Object("", "src/textures/gui/play/heartFull.bmp", guiObjs[0], true);

	//boss unlocks
	guiObjs[29] = new Object("", "src/textures/gui/progress/doublejump.bmp", guiObjs[0], true);
	guiObjs[30] = new Object("", "src/textures/gui/progress/webclimp.bmp", guiObjs[0], true);
	guiObjs[31] = new Object("", "src/textures/gui/progress/mirrorwalk.bmp", guiObjs[0], true);


	for (int n = 32; n < 50; n++)
		guiObjs[n] = 0;

	// edit reserved 50 - 59
	guiObjs[50] = new Object("", "src/textures/gui/edit/editHud.bmp", guiObjs[0], true);
	guiObjs[51] = new Object("", "src/textures/gui/edit/switch.bmp", guiObjs[0], true);
	guiObjs[52] = new Object("", "src/textures/gui/edit/switchON.bmp", guiObjs[0], true);

	for (int n = 53; n < 60; n++)
		guiObjs[n] = 0;


	// intro reserved  60 - 75
	guiObjs[60] = new Object("", "src/textures/gui/intro/into0.bmp", guiObjs[0], true);
	guiObjs[61] = new Object("", "src/textures/gui/intro/into1.bmp", guiObjs[0], true);
	guiObjs[62] = new Object("", "src/textures/gui/intro/dream1.bmp", guiObjs[0], true);
	guiObjs[63] = new Object("", "src/textures/gui/intro/dream2.bmp", guiObjs[0], true);
	guiObjs[64] = new Object("", "src/textures/gui/intro/dream3.bmp", guiObjs[0], true);
	guiObjs[65] = new Object("", "src/textures/gui/intro/intro2.bmp", guiObjs[0], true);


	for (int n = 66; n < 75; n++)
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