#include "ContentManager.h"

ContentManager::~ContentManager()
{
	if (guiObjs)
	{
		for (int i = 0; i < guiCount; i++)
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

	guiCount = 100;
	guiObjs = new Object*[guiCount]();

	//main menu reserved 0 - 19
	guiObjs[0] = new Object("src/meshes/BaseBlit.v", "src/textures/gui/main/logo.png");
	guiObjs[1] = new Object("", "src/textures/gui/main/newgame.png", guiObjs[0], true);
	guiObjs[2] = new Object("", "src/textures/gui/main/newgameH.png", guiObjs[0], true);
	guiObjs[3] = new Object("", "src/textures/gui/main/continue.png", guiObjs[0], true);
	guiObjs[4] = new Object("", "src/textures/gui/main/continueH.png", guiObjs[0], true);
	guiObjs[5] = new Object("", "src/textures/gui/main/continueG.png", guiObjs[0], true);
	guiObjs[6] = new Object("", "src/textures/gui/main/mapmaker.png", guiObjs[0], true);
	guiObjs[7] = new Object("", "src/textures/gui/main/mapmakerH.png", guiObjs[0], true);
	guiObjs[8] = new Object("", "src/textures/gui/main/credits.png", guiObjs[0], true);
	guiObjs[9] = new Object("", "src/textures/gui/main/creditsH.png", guiObjs[0], true);
	guiObjs[10] = new Object("", "src/textures/gui/main/settings.png", guiObjs[0], true);
	guiObjs[11] = new Object("", "src/textures/gui/main/settingsH.png", guiObjs[0], true);
	guiObjs[12] = new Object("", "src/textures/gui/main/settings/audioE.bmp", guiObjs[0], true);
	guiObjs[13] = new Object("", "src/textures/gui/main/settings/audioD.bmp", guiObjs[0], true);
	guiObjs[14] = new Object("", "src/textures/gui/main/settings/musicE.bmp", guiObjs[0], true);
	guiObjs[15] = new Object("", "src/textures/gui/main/settings/musicD.bmp", guiObjs[0], true);
	guiObjs[16] = new Object("", "src/textures/gui/main/settings/soundE.bmp", guiObjs[0], true);
	guiObjs[17] = new Object("", "src/textures/gui/main/settings/soundD.bmp", guiObjs[0], true);
	for (int n = 18; n < 20; n++)
		guiObjs[n] = 0;

	//paused, exit to menu
	guiObjs[20] = new Object("", "src/textures/gui/pause/paused.png", guiObjs[0], true);
	guiObjs[21] = new Object("", "src/textures/gui/pause/exit.png", guiObjs[0], true);
	guiObjs[22] = new Object("", "src/textures/gui/pause/exitH.png", guiObjs[0], true);

	//boss progress
	guiObjs[23] = new Object("", "src/textures/gui/progress/bosses.png", guiObjs[0], true);
	guiObjs[24] = new Object("", "src/textures/gui/progress/defeated.png", guiObjs[0], true);

	// game play reserved 25 - 49
	//hud
	guiObjs[25] = new Object("", "src/textures/gui/play/skeletonHUD.png", guiObjs[0], true);
	guiObjs[26] = new Object("", "src/textures/gui/play/heart1.png", guiObjs[0], true);
	guiObjs[27] = new Object("", "src/textures/gui/play/heart2.png", guiObjs[0], true);
	guiObjs[28] = new Object("", "src/textures/gui/play/heartFull.png", guiObjs[0], true);

	//boss unlocks
	guiObjs[29] = new Object("", "src/textures/gui/progress/doublejump.png", guiObjs[0], true);
	guiObjs[30] = new Object("", "src/textures/gui/progress/webclimp.png", guiObjs[0], true);
	guiObjs[31] = new Object("", "src/textures/gui/progress/mirrorwalk.png", guiObjs[0], true);

	//rune displays
	guiObjs[32] = new Object("", "src/textures/gui/play/flameRune.png", guiObjs[0], true);
	guiObjs[33] = new Object("", "src/textures/gui/play/sparkRune.png", guiObjs[0], true);
	guiObjs[34] = new Object("", "src/textures/gui/play/forceHeart.png", guiObjs[0], true);

	//boss quotes
	guiObjs[35] = new Object("", "src/textures/gui/progress/bossentrance.png", guiObjs[0], true);
	guiObjs[36] = new Object("", "src/textures/gui/progress/finalform.png", guiObjs[0], true);
	guiObjs[37] = new Object("", "src/textures/gui/progress/bossdeath.png", guiObjs[0], true);


	for (int n = 38; n < 50; n++)
		guiObjs[n] = 0;

	// edit reserved 50 - 59
	guiObjs[50] = new Object("", "src/textures/gui/edit/editHud.png", guiObjs[0], true);
	guiObjs[51] = new Object("", "src/textures/gui/edit/switch.bmp", guiObjs[0], true);
	guiObjs[52] = new Object("", "src/textures/gui/edit/switchON.bmp", guiObjs[0], true);

	for (int n = 53; n < 60; n++)
		guiObjs[n] = 0;


	// intro reserved  60 - 75
	guiObjs[60] = new Object("", "src/textures/gui/intro/into0.png", guiObjs[0], true);
	guiObjs[61] = new Object("", "src/textures/gui/intro/into1.png", guiObjs[0], true);
	guiObjs[62] = new Object("", "src/textures/gui/intro/dream1.png", guiObjs[0], true);
	guiObjs[63] = new Object("", "src/textures/gui/intro/dream2.png", guiObjs[0], true);
	guiObjs[64] = new Object("", "src/textures/gui/intro/dream3.png", guiObjs[0], true);
	guiObjs[65] = new Object("", "src/textures/gui/intro/intro2.png", guiObjs[0], true);


	for (int n = 66; n < 75; n++)
		guiObjs[n] = 0;

	// difficulty reserved
	guiObjs[75] = new Object("", "src/textures/gui/main/settings/casual.png", guiObjs[0], true);
	guiObjs[76] = new Object("", "src/textures/gui/main/settings/casualH.png", guiObjs[0], true);
	guiObjs[77] = new Object("", "src/textures/gui/main/settings/hardcore.png", guiObjs[0], true);
	guiObjs[78] = new Object("", "src/textures/gui/main/settings/hardcoreH.png", guiObjs[0], true);
	guiObjs[79] = new Object("", "src/textures/gui/main/settings/dmoninhell.png", guiObjs[0], true);
	guiObjs[80] = new Object("", "src/textures/gui/main/settings/dmoninhellH.png", guiObjs[0], true);
	guiObjs[81] = new Object("", "src/textures/gui/main/settings/difficulty.png", guiObjs[0], true);
	for (int n = 82; n < 100; n++)
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