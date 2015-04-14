#include "ContentManager.h"

ContentManager::~ContentManager()
{
	if (player[0])
		delete player[0];
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
	if (player[0]) //only call once
		throw;

	player[0] = new Object("src/meshes/PlayerBase.v", "src/textures/HEIL.bmp");
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
	player[0]->bind();
	return player[0]->getFaces();
}

int ContentManager::bindMapObj(int id) const
{
	mapObjs[id]->bind();
	return mapObjs[id]->getFaces();
}