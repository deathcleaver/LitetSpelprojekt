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
	loadObjManager();
	loadPlayer();
	loadMapObjs();
	loadMonsterObjs();
	
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

void ContentManager::loadPlayer()
{
	if (player) //only call once
		throw;
	//this gon be two objects
	Object** playerAn = new Object*[2];
	playerAn[0] = new Object("src/meshes/PlayerBase.v", "src/textures/black.bmp");
	playerAn[1] = new Object("src/meshes/PlayerWalk.v", "", playerAn[0], false, true);
	player = new Object("src/meshes/PlayerWalk.v", "src/textures/black.bmp");//new Animation(playerAn, 2, .5f);// = new Object("src/meshes/PlayerBase.v", "src/textures/HEIL.bmp");

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

	guiCount = 1;
	guiObjs = new Object*[guiCount]();

	guiObjs[0] = new Object("src/meshes/BaseBlit.v", "src/textures/gui/pause.bmp");
}

int ContentManager::bindPlayer() const
{
	//player->updateWeight();
	/*player->bind();
	return player->getFaces();*/
	return objectManager->bindPlayer();
}

int ContentManager::bindMapObj(int id) const
{
	/*
	mapObjs[id]->bind();
	return mapObjs[id]->getFaces();*/
	return objectManager->bindMapObj(id);
}

int ContentManager::bindMonsterObj(int id) const
{
	/*monsterObjs[id]->bind();
	return monsterObjs[id]->getFaces();*/
	return objectManager->bindMonsterObj(id);
}

void ContentManager::bindGUItex(int id) const
{
	guiObjs[id]->bindTexOnly();
}

void ContentManager::bindGUIvert() const
{
	guiObjs[0]->bindVertOnly();
}

void ContentManager::setPlayerState(std::string state)
{
	objectManager->setPlayerState(state);
}