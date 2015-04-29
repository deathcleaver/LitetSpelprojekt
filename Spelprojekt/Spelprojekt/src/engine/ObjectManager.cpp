#include"ObjectManager.h"

ObjectManager::ObjectManager()
{
	objects = vector<vector<AnimationObject*>>();
	vector<AnimationObject*> myBackObjs = vector<AnimationObject*>();
	vector<AnimationObject*> myMapObjs = vector<AnimationObject*>();
	vector<AnimationObject*> myMiscObjs = vector<AnimationObject*>();
	vector<AnimationObject*> myMonsterObjs = vector<AnimationObject*>();
	
	objects.push_back(myBackObjs);
	objects.push_back(myMapObjs);
	objects.push_back(myMiscObjs);
	objects.push_back(myMonsterObjs);
}

ObjectManager::~ObjectManager()
{
	if (collisionRekt)
		delete collisionRekt;

	if (myPlayer != nullptr)
		delete myPlayer;

	for (int n = 0; n < back_count; n++)
	{
		delete objects[BACK][n];
	}
	for (int n = 0; n < world_count; n++)
	{
		delete objects[WORLD][n];
	}
	for (int n = 0; n < misc_count; n++)
	{
		delete objects[MISC][n];
	}
	for (int n = 0; n < enemy_count; n++)
	{
		delete objects[ENEMY][n];
	}
}

void ObjectManager::init()
{
	loadPlayer();
	loadBackObjs();
	loadMapObjs();
	loadMiscObjs();
	loadMonsterObjs();

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
	AnimationObject* add;

	//box
	add = new AnimationObject("src/meshes/BaseCube.v", "src/textures/black.bmp");
	objects[OBJ::WORLD].push_back(add);
	
	//mushroom
	add = new AnimationObject("src/meshes/Background Objects/MushroomTest.v", "src/textures/Background Objects/MushroomTexture2.bmp");
	objects[OBJ::WORLD].push_back(add);
	
	//stalactite
	add = new AnimationObject("src/meshes/Background Objects/Stalactite.v", "src/textures/Background Objects/Stalactimite.bmp");
	objects[OBJ::WORLD].push_back(add);

	//rock1x1
	add = new AnimationObject("src/meshes/Background Objects/Rock1x1.v", "src/textures/Background Objects/Stalactimite.bmp");
	objects[OBJ::WORLD].push_back(add);

	//minecart
	add = new AnimationObject("src/meshes/Background Objects/MeinKart.v", "src/textures/Background Objects/MeinKart.bmp");
	objects[OBJ::WORLD].push_back(add);

	//minetrack
	add = new AnimationObject("src/meshes/Background Objects/Minetrack.v", "src/textures/Background Objects/Minetrack.bmp");
	objects[OBJ::WORLD].push_back(add);

	//minestruct_small 
	add = new AnimationObject("src/meshes/Background Objects/MineStrutSmall.v", "src/textures/Background Objects/MineStrut.bmp");
	objects[OBJ::WORLD].push_back(add);

	//minestruct_large 
	add = new AnimationObject("src/meshes/Background Objects/MineStrutLarge.v", "src/textures/Background Objects/MineStrut.bmp");
	objects[OBJ::WORLD].push_back(add);

	//lantern
	add = new AnimationObject("src/meshes/Background Objects/Lantern.v", "src/textures/Background Objects/Lantern.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform rock 1x1
	add = new AnimationObject("src/meshes/Map Blocks/Platform1x1.v", "src/textures/Background/Platform.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform rock 1x3
	add = new AnimationObject("src/meshes/Map Blocks/Platform1x3.v", "src/textures/Background/Platform.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform rock 1x5
	add = new AnimationObject("src/meshes/Map Blocks/Platform1x5.v", "src/textures/Background/Platform.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform rock 3x3
	add = new AnimationObject("src/meshes/Map Blocks/Platform3x3.v", "src/textures/Background/Platform.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform rock 5x5
	add = new AnimationObject("src/meshes/Map Blocks/Platform5x5.v", "src/textures/Background/Platform.bmp");
	objects[OBJ::WORLD].push_back(add);

	//rock5x5
	add = new AnimationObject("src/meshes/Background Objects/Rock5x5.v", "src/textures/Background Objects/Stalactimite.bmp");
	objects[OBJ::WORLD].push_back(add);

	//rock5x5 cracked
	add = new AnimationObject("src/meshes/Background Objects/Rock5x5Crack.v", "src/textures/Background Objects/Stalactimite.bmp");
	objects[OBJ::WORLD].push_back(add);

	//mushroom wall
	add = new AnimationObject("src/meshes/Background Objects/MushroomTicka.v", "src/textures/Background Objects/MushroomTicka.bmp");
	objects[OBJ::WORLD].push_back(add);
}

void ObjectManager::loadMonsterObjs()
{
	AnimationObject* add;

	//spikes
	add = new AnimationObject("src/meshes/Enemies/SpikeBase.v", "src/textures/Red.bmp");
	objects[OBJ::ENEMY].push_back(add);

	//bats
	Object* bat1 = new Object("src/meshes/Enemies/BatWingsBackFix.v", "src/textures/Red.bmp");
	Object* bat2 = new Object("src/meshes/Enemies/BatWingsForwardFix.v", "src/textures/Red.bmp");
	add = new AnimationObject(bat1, bat2, 0.5f, 0.08f);
	objects[OBJ::ENEMY].push_back(add);

	//flame
	Object* FCube1 = new Object("src/meshes/Enemies/FlameCubeContracted.v", "src/textures/Red.bmp");
	Object* FCube2 = new Object("src/meshes/Enemies/FlameCubeExtruded.v", "src/textures/Red.bmp");
	add = new AnimationObject(FCube1, FCube2, 0.5f, 0.02f);
	objects[OBJ::ENEMY].push_back(add);
}

void ObjectManager::loadMiscObjs()
{
	AnimationObject* add;

	//shrine
	add = new AnimationObject("src/meshes/Interactive Objects/Shrine.v", "src/textures/Interactive Objects/Shrine2.bmp");
	objects[OBJ::MISC].push_back(add);

	//rune_range
	add = new AnimationObject("src/meshes/Interactive Objects/RuneAttack.v", "src/textures/Interactive Objects/Rune-Attack.bmp");
	objects[OBJ::MISC].push_back(add);

	//rune_damage
	add = new AnimationObject("src/meshes/Interactive Objects/RuneDamage.v", "src/textures/Interactive Objects/Rune-Damage.bmp");
	objects[OBJ::MISC].push_back(add);

	//rune_shield
	add = new AnimationObject("src/meshes/Interactive Objects/RuneShield.v", "src/textures/Interactive Objects/Rune-Shield.bmp");
	objects[OBJ::MISC].push_back(add);
}

void ObjectManager::loadBackObjs()
{
	AnimationObject* add;

	//grid
	add = new AnimationObject("src/meshes/planeVerticalF.v", "src/textures/Background/grid.bmp");
	objects[OBJ::BACK].push_back(add);

	//rock_grey
	add = new AnimationObject("src/meshes/Background/BackgroundRockWallFix.v", "src/textures/Background/BackgroundRockWall.bmp");
	objects[OBJ::BACK].push_back(add);

	//rock_dirt
	add = new AnimationObject("src/meshes/Background/BackgroundRockWallFix.v", "src/textures/Background/BackgroundRockWallDIRT.bmp");
	objects[OBJ::BACK].push_back(add);

	//rock_boss
	add = new AnimationObject("src/meshes/Background/BackgroundBat.v", "src/textures/Background/BackgroundBat.bmp");
	objects[OBJ::BACK].push_back(add);
}

void ObjectManager::update()
{
	myPlayer->update();
	for (int i = 0; i < WorldID::world_count ; i++)
	{
		objects[WORLD][i]->update();
	}
	for (int i = 0; i < EnemyID::enemy_count; i++)
	{
		objects[ENEMY][i]->update();
	}
	for (int i = 0; i < MiscID::misc_count; i++)
	{
		objects[MISC][i]->update();
	}
}

int ObjectManager::bindPlayer() const
{
	//myPlayer->update();
	myPlayer->bindAnimObject();
	return myPlayer->getFaces();
}

int ObjectManager::bind(int type, int id) const
{
	objects[type][id]->bindAnimObject();
	return objects[type][id]->getFaces();
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

int ObjectManager::nrOfWorldItems() const
{
	return WorldID::world_count;
}

int ObjectManager::nrOfMonsterItems() const
{
	return EnemyID::enemy_count;
}