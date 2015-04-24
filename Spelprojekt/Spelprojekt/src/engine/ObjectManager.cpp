#include"ObjectManager.h"

ObjectManager::ObjectManager()
{
	myPlayer = nullptr;

	myMapObjs = nullptr;

	myMonsterObjs = nullptr;
}

ObjectManager::~ObjectManager()
{
	if (collisionRekt)
		delete collisionRekt;

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
	
	collisionRekt = new AnimationObject("src/meshes/Rekt.v", "src/textures/pink.bmp");
}

void ObjectManager::loadPlayer()
{
	Object** playerAn = new Object*[5];
	playerAn[0] = new Object("src/meshes/Player/PlayerBaseSword2.v", "src/textures/blue.bmp");
	playerAn[1] = new Object("src/meshes/Player/PlayerWalkLeft.v", "", playerAn[0], false, true);
	playerAn[2] = new Object("src/meshes/Player/PlayerWalkRight.v", "", playerAn[0], false, true);
	playerAn[3] = new Object("src/meshes/Player/PlayerAttackUp.v", "", playerAn[0], false, true);
	playerAn[4] = new Object("src/meshes/Player/PlayerAttackDown.v", "", playerAn[0], false, true);

	myPlayer = new AnimationObject(playerAn, "src/meshes/Player/PlayerBaseSword2.v", 5, 0.5f, 0.05f);

}

void ObjectManager::loadMapObjs()
{
	//to be added

	if (myMapObjs) //only call once
		throw;

	myNrOfMapObjects = 14;
	myMapObjs = new AnimationObject*[myNrOfMapObjects]();

	//backgroudn grid
	myMapObjs[0] = new AnimationObject("src/meshes/planeVerticalF.v", "src/textures/Background/grid.bmp");


	//basic platform
	myMapObjs[1] = new AnimationObject("src/meshes/BaseCube.v", "src/textures/Background/BackgroundRockWallDIRT.bmp");


	//shrine
	myMapObjs[2] = new AnimationObject("src/meshes/Interactive Objects/Shrine.v", "src/textures/Interactive Objects/Shrine2.bmp");

	//mushroom
	myMapObjs[3] = new AnimationObject("src/meshes/Background OBjects/MushroomTest.v", "src/textures/Background Objects/MushroomTexture2.bmp");

	//Attack runes
	myMapObjs[4] = new AnimationObject("src/meshes/Interactive Objects/RuneAttack.v", "src/textures/Interactive Objects/Rune-Attack.bmp");

	//Damage rune
	myMapObjs[5] = new AnimationObject("src/meshes/Interactive Objects/RuneDamage.v", "src/textures/Interactive Objects/Rune-Damage.bmp");

	//Shield rune
	myMapObjs[6] = new AnimationObject("src/meshes/Interactive Objects/RuneShield.v", "src/textures/Interactive Objects/Rune-Shield.bmp");

	//Grey rock wall
	myMapObjs[7] = new AnimationObject("src/meshes/Background/BackgroundRockWallFix.v", "src/textures/Background/BackgroundRockWall.bmp");
	
	//Brown rock wall
	myMapObjs[8] = new AnimationObject("src/meshes/Background/BackgroundRockWallFix.v", "src/textures/Background/BackgroundRockWallDIRT.bmp");

	//Batboss special wall
	myMapObjs[9] = new AnimationObject("src/meshes/Background/BackgroundBat.v", "src/textures/Background/BackgroundBat.bmp");

	//stalactite
	myMapObjs[10] = new AnimationObject("src/meshes/Background Objects/Stalactite.v", "src/textures/Background Objects/Stalactimite.bmp");

	//stalagmite
	myMapObjs[11] = new AnimationObject("src/meshes/Background Objects/Stalagmite.v", "src/textures/Background Objects/Stalactimite.bmp");

	//Minecart
	myMapObjs[12] = new AnimationObject("src/meshes/Background Objects/MeinKart.v", "src/textures/Background Objects/MeinKart.bmp");

	//Minetrack
	myMapObjs[13] = new AnimationObject("src/meshes/Background Objects/Minetrack.v", "src/textures/Background Objects/Minetrack.bmp");

}

void ObjectManager::loadMonsterObjs()
{
	if (myMonsterObjs)
		throw;

	myNrOfMonsterObjects = 3;
	myMonsterObjs = new AnimationObject*[myNrOfMonsterObjects]();

	//spikes
	myMonsterObjs[0] = new AnimationObject("src/meshes/Enemies/SpikeBase.v", "src/textures/Red.bmp");


	//bats
	Object* bat1 = new Object("src/meshes/Enemies/BatWingsBackFix.v", "src/textures/Red.bmp");
	Object* bat2 = new Object("src/meshes/Enemies/BatWingsForwardFix.v", "src/textures/Red.bmp");
	myMonsterObjs[1] = new AnimationObject(bat1, bat2, 0.5f, 0.08f);


	//flamecubes
	Object* FCube1 = new Object("src/meshes/Enemies/FlameCubeContracted.v", "src/textures/Red.bmp");
	Object* FCube2 = new Object("src/meshes/Enemies/FlameCubeExtruded.v", "src/textures/Red.bmp");
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

void ObjectManager::bindRekt() const
{
	collisionRekt->bindAnimObject();
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
		myPlayer->setAnimPoints(1, 2);
		myPlayer->setSpeed(0.02f);
	}
	else if (state == "flinch")
	{
		myPlayer->setAnimPoints(2, 2);
		myPlayer->setSpeed(0.02f);
	}
	else if (state == "attack")
	{
		myPlayer->setAnimPoints(3, 4);
		myPlayer->setSpeed(0.08f);
	}
}