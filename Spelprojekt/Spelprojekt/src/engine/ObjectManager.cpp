#include"ObjectManager.h"

ObjectManager::ObjectManager()
{
	myPlayer = nullptr;

	myMapObjs = nullptr;

	myMonsterObjs = nullptr;
}

ObjectManager::~ObjectManager()
{
	if (myPlayer != nullptr)
		delete myPlayer;

	if (myMapObjs != nullptr)
	{
		for (int i = 0; i < myNrOfMapObjects; i++)
		{
			delete myMapObjs[i];
		}
		delete[] myMapObjs;
	}

	if (myMonsterObjs != nullptr)
	{
		for (int i = 0; i < myNrOfMonsterObjects; i++)
		{
			delete myMonsterObjs[i];
		}
		delete[] myMonsterObjs;
	}
}

void ObjectManager::init()
{
	loadMonsterObjs();
	loadPlayer();

	loadMapObjs();


}

void ObjectManager::loadPlayer()
{
	Object** playerAn = new Object*[5];
	playerAn[0] = new Object("src/meshes/PlayerBaseSword2.v", "src/textures/red.bmp");
	playerAn[1] = new Object("src/meshes/PlayerWalkLeft.v", "", playerAn[0], false, true);
	playerAn[2] = new Object("src/meshes/PlayerWalkRight.v", "", playerAn[0], false, true);
	playerAn[3] = new Object("src/meshes/PlayerAttackUp.v", "", playerAn[0], false, true);
	playerAn[4] = new Object("src/meshes/PlayerAttackDown.v", "", playerAn[0], false, true);

	myPlayer = new AnimationObject(playerAn, "src/meshes/PlayerBaseSword2.v", 5, 0.5f, 0.05f);

}

void ObjectManager::loadMapObjs()
{
	//to be added

	if (myMapObjs) //only call once
		throw;

	myNrOfMapObjects = 3;
	myMapObjs = new AnimationObject*[myNrOfMapObjects]();

	//backgroudn grid
	myMapObjs[0] = new AnimationObject("src/meshes/planeVerticalF.v", "src/textures/grid.bmp");


	//basic platform
	myMapObjs[1] = new AnimationObject("src/meshes/BaseCube.v", "src/textures/mudtest.bmp");


	//shrine
	myMapObjs[2] = new AnimationObject("src/meshes/PlayerBase.v", "src/textures/black.bmp");
}

void ObjectManager::loadMonsterObjs()
{
	if (myMonsterObjs)
		throw;

	myNrOfMonsterObjects = 3;
	myMonsterObjs = new AnimationObject*[myNrOfMonsterObjects]();

	//spikes
	myMonsterObjs[0] = new AnimationObject("src/meshes/SpikeBase.v", "src/textures/Red.bmp");


	//bats
	Object* bat1 = new Object("src/meshes/BatWingsBackFix.v", "src/textures/Red.bmp");
	Object* bat2 = new Object("src/meshes/BatWingsForwardFix.v", "src/textures/Red.bmp");
	myMonsterObjs[1] = new AnimationObject(bat1, bat2, 0.5f, 0.05f);


	//flamecubes
	Object* FCube1 = new Object("src/meshes/FlameCubeContracted.v", "src/textures/Red.bmp");
	Object* FCube2 = new Object("src/meshes/FlameCubeExtruded.v", "src/textures/Red.bmp");
	myMonsterObjs[2] = new AnimationObject(FCube1, FCube2, 0.5f, 0.02f);

}

void ObjectManager::update()
{
	myPlayer->update();
	for (int i = 0; i < myNrOfMapObjects; i++)
	{
		myMapObjs[i]->update();
	}
	for (int i = 0; i < myNrOfMonsterObjects; i++)
	{
		myMonsterObjs[i]->update();
	}
}

int ObjectManager::bindPlayer() const
{
	//myPlayer->update();
	myPlayer->bindAnimObject();
	return myPlayer->getFaces();
}

int ObjectManager::bindMapObj(int id) const
{
	//myMapObjs[id]->update();
	myMapObjs[id]->bindAnimObject();
	return myMapObjs[id]->getFaces();
}

int ObjectManager::bindMonsterObj(int id) const
{
	//myMonsterObjs[id]->update();
	myMonsterObjs[id]->bindAnimObject();
	return myMonsterObjs[id]->getFaces();
}

void ObjectManager::setPlayerState(std::string state)
{
	if (state == "idle")
	{
		myPlayer->setAnimPoints(0, 0);
		myPlayer->setSpeed(0.02f);
	}
	else if (state == "walk")
	{
		myPlayer->setAnimPoints(1, 2);
		myPlayer->setSpeed(0.02f);
	}
	else if (state == "air")
	{
		myPlayer->setAnimPoints(0, 1);
		myPlayer->setSpeed(0.02f);
	}
	else if (state == "flinch")
	{
		myPlayer->setAnimPoints(0, 1);
		myPlayer->setSpeed(0.02f);
	}
	else if (state == "attack")
	{
		myPlayer->setAnimPoints(3, 4);
		myPlayer->setSpeed(0.08f);
	}
}