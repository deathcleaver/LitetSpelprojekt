#include "ContentManager.h"

ContentManager::~ContentManager()
{
	if (player)
		delete player;
	if (mapObjs)
	{
		for (int n = 0; n < mapObjCount; n++)
		{
			delete mapObjs[n];
		}
		delete[] mapObjs;
	}
	if (monsterObjs)
	{
		for (int c = 0; c < monsterObjCount; c++)
		{
			delete monsterObjs[c];
		}
		delete[]monsterObjs;
	}
}

void ContentManager::init()
{
	//load all player meshes
	loadPlayer();

	//load all map meshes
	loadMapObjs();

	//load all monster meshes
	loadMonsterObjs();
	
	//load all gui textures
	loadGUIobjs();

}

void ContentManager::loadPlayer()
{
	if (player) //only call once
		throw;

	player = new Object("src/meshes/PlayerBase.v", "src/textures/red.bmp");
}

void ContentManager::loadMapObjs()
{
	if (mapObjs) //only call once
		throw; 

	mapObjCount = 3; 
	mapObjs = new Object*[mapObjCount]();

	mapObjs[0] = new Object("src/meshes/planeVerticalF.v", "src/textures/grid.bmp");
	mapObjs[1] = new Object("src/meshes/BaseCube.v", "src/textures/mudtest.bmp");
	mapObjs[2] = new Object("src/meshes/PlayerBase.v", "src/textures/black.bmp"); //Shrine

}

void ContentManager::loadMonsterObjs()
{
	if (monsterObjs)
		throw;

	monsterObjCount = 3;
	monsterObjs = new Object*[monsterObjCount]();

	monsterObjs[0] = new Object("src/meshes/BaseCube.v", "src/textures/grid.bmp");
	monsterObjs[1] = new Object("src/meshes/BaseCube.v", "src/textures/Red.bmp");
	monsterObjs[2] = new Object("src/meshes/BaseCube.v", "src/textures/black.bmp");
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
	for (int n = 51; n < 75; n++)
		guiObjs[n] = 0;
}

int ContentManager::bindPlayer() const
{
	player->bind();
	return player->getFaces();
}

int ContentManager::bindMapObj(int id) const
{
	mapObjs[id]->bind();
	return mapObjs[id]->getFaces();
}

int ContentManager::bindMonsterObj(int id) const
{
	monsterObjs[id]->bind();
	return monsterObjs[id]->getFaces();
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