#include "ContentManager.h"

void ContentManager::init()
{
	//load all player meshes
	loadPlayer();
	
	//load all monster meshes

	//load all map meshes


}

ContentManager::~ContentManager()
{
	if (player)
		delete player;
}

void ContentManager::loadPlayer()
{
	if (!player)
		player = new Object("testBoat.v", "white.bmp");
}

int ContentManager::bindPlayer() const
{
	player->bind();
	return player->getFaces();
}