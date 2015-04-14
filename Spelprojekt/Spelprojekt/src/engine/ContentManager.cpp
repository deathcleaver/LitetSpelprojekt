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
}

void ContentManager::init()
{
	//load all player meshes
	loadPlayer();

	//load all map meshes
	loadMapObjs();

	//load all monster meshes

}

void ContentManager::loadPlayer()
{
	if (player) //only call once
		throw;

	player = new Object("src/meshes/testBoat.v", "src/textures/boat.bmp");
}

void ContentManager::loadMapObjs()
{
	if (mapObjs) //only call once
		throw; 

	mapObjCount = 2; 
	mapObjs = new Object*[mapObjCount]();

	mapObjs[0] = new Object("src/meshes/planeVerticalF.v", "src/textures/grid.bmp");
	mapObjs[1] = new Object("src/meshes/BaseCube.v", "src/textures/black.bmp");

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