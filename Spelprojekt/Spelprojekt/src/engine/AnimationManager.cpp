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

}

void AnimationManager::update()
{

}

int AnimationManager::bindPlayer() const
{
	return 0;
}

int AnimationManager::bindMapObj(int id) const
{
	return 0;
}