#include"AnimationManager.h"

AnimationManager::AnimationManager()
{
	myPlayer = nullptr;

	myAnimatedObjects = nullptr;
}

AnimationManager::~AnimationManager()
{
	if (myPlayer != nullptr)
		delete myPlayer;

	if (myAnimatedObjects != nullptr)
	{
		for (int i = 0; i < myNrOfObjects; i++)
		{
			delete myAnimatedObjects[i];
		}
		delete[] myAnimatedObjects;
	}
}

void AnimationManager::init()
{
	loadPlayer();

	loadMapObjs();
}

void AnimationManager::loadPlayer()
{
	Object** playerAn = new Object*[2];
	playerAn[0] = new Object("src/meshes/PlayerBase.v", "src/textures/black.bmp");
	playerAn[1] = new Object("src/meshes/PlayerWalk.v", "", playerAn[0], false, true);

	myPlayer = new AnimationObject(playerAn, 2, 0.5f, 0.05f);
}

void AnimationManager::loadMapObjs()
{
	//to be added
	/*
	if (myMapObjs) //only call once
		throw;

	mapObjCount = 3;
	mapObjs = new Object*[mapObjCount]();

	mapObjs[0] = new Object("src/meshes/planeVerticalF.v", "src/textures/grid.bmp");
	mapObjs[1] = new Object("src/meshes/BaseCube.v", "src/textures/mudtest.bmp");
	mapObjs[2] = new Object("src/meshes/PlayerBase.v", "src/textures/black.bmp"); //Shrine*/
}

void AnimationManager::update()
{
	myPlayer->update();

	//Add mapObj updates
}

int AnimationManager::bindPlayer() const
{
	myPlayer->bindAnimObject();
	return myPlayer->getFaces();
}

int AnimationManager::bindMapObj(int id) const
{
	//to be added
	return 0;
}