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
	playerAn[0] = new Object("src/meshes/Player/PlayerIdle.v", "src/textures/Player.bmp");
	playerAn[1] = new Object("src/meshes/Player/PlayerWalkSwordFront2.v", "", playerAn[0], false, true);
	playerAn[2] = new Object("src/meshes/Player/PlayerWalkSwordBack2.v", "", playerAn[0], false, true);
	playerAn[3] = new Object("src/meshes/Player/PlayerAttackDown.v", "", playerAn[0], false, true);
	playerAn[4] = new Object("src/meshes/Player/PlayerAttackUp.v", "", playerAn[0], false, true);


	myPlayer = new AnimationObject(playerAn, "src/meshes/Player/PlayerIdle.v", 5, 0.5f, 0.05f);
	//myPlayer = new AnimationObject("src/meshes/Player/PlayerIdle.v", "src/textures/player.bmp");

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

	//pickaxe
	add = new AnimationObject("src/meshes/Background Objects/Pickaxe.v", "src/textures/Background Objects/pickaxe.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Board
	add = new AnimationObject("src/meshes/Background Objects/Board.v", "src/textures/Background Objects/Minestrut.bmp");
	objects[OBJ::WORLD].push_back(add);

	//crate HD
	add = new AnimationObject("src/meshes/Background Objects/Crate.v", "src/textures/Background Objects/crate.bmp");
	objects[OBJ::WORLD].push_back(add);

	//crate 3D
	add = new AnimationObject("src/meshes/Background Objects/Crate2.v", "src/textures/Background Objects/crate2.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform marble 1x1
	add = new AnimationObject("src/meshes/Map Blocks/PlatformMarble1x1.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform marble 1x3
	add = new AnimationObject("src/meshes/Map Blocks/PlatformMarble1x3.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform marble 1x5
	add = new AnimationObject("src/meshes/Map Blocks/PlatformMarble1x5.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform marble 3x3
	add = new AnimationObject("src/meshes/Map Blocks/PlatformMarble3x3.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Platform marble 5x5
	add = new AnimationObject("src/meshes/Map Blocks/PlatformMarble5x5.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//candle_big
	add = new AnimationObject("src/meshes/Crypt/CandleBig.v", "src/textures/white.bmp");
	objects[OBJ::WORLD].push_back(add);

	//candle_small
	add = new AnimationObject("src/meshes/Crypt/CandleSmall.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//candelier
	add = new AnimationObject("src/meshes/Crypt/Chandelier.v", "src/textures/Background Objects/steel.bmp");
	objects[OBJ::WORLD].push_back(add);

	//coffin_bottom
	add = new AnimationObject("src/meshes/Crypt/CoffinBottom.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//coffin_top
	add = new AnimationObject("src/meshes/Crypt/CoffinTop.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//grave_circle
	add = new AnimationObject("src/meshes/Crypt/GraveCircle.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//grave_classic
	add = new AnimationObject("src/meshes/Crypt/GraveClassic.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//grave_cross
	add = new AnimationObject("src/meshes/Crypt/GraveCross.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//pillar_bottom_broken
	add = new AnimationObject("src/meshes/Crypt/PillarBottomBroken.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//pillar_top_broken
	add = new AnimationObject("src/meshes/Crypt/PillarTopBroken.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//pillar
	add = new AnimationObject("src/meshes/Crypt/PillarWhole.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//tunnel_broken
	add = new AnimationObject("src/meshes/Crypt/TunnelBroken.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//tunnel
	add = new AnimationObject("src/meshes/Crypt/TunnelWhole.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//red block
	add = new AnimationObject("src/meshes/BaseCube.v", "src/textures/carpet.bmp");
	objects[OBJ::WORLD].push_back(add);

	//marble shrine
	add = new AnimationObject("src/meshes/Interactive Objects/Shrine.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//dead_tree
	add = new AnimationObject("src/meshes/overworld/DeadTree.v", "src/textures/Background Objects/deadtree.bmp");
	objects[OBJ::WORLD].push_back(add);

	//fence
	add = new AnimationObject("src/meshes/overworld/Fence.v", "src/textures/Background Objects/fenceTex.bmp");
	objects[OBJ::WORLD].push_back(add);

	//gate
	add = new AnimationObject("src/meshes/overworld/Gate.v", "src/textures/Background Objects/gate.bmp");
	objects[OBJ::WORLD].push_back(add);

	//mirror
	add = new AnimationObject("src/meshes/overworld/Mirror.v", "src/textures/Background/marble.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Chair
	add = new AnimationObject("src/meshes/Ghost/Chair.v", "src/textures/Ghost/chair.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Table
	add = new AnimationObject("src/meshes/Ghost/Table.v", "src/textures/Ghost/chair.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Bookshelf Empty
	add = new AnimationObject("src/meshes/Ghost/BookshelfEmpty.v", "src/textures/Ghost/Hylla.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Bookshelf Full
	add = new AnimationObject("src/meshes/Ghost/BookshelfFull.v", "src/textures/Ghost/Hylla.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Book Red
	add = new AnimationObject("src/meshes/Ghost/Book.v", "src/textures/Ghost/red_book.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Book Blue
	add = new AnimationObject("src/meshes/Ghost/Book.v", "src/textures/Ghost/Blue_book.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Book Green
	add = new AnimationObject("src/meshes/Ghost/Book.v", "src/textures/Ghost/Green_book.bmp");
	objects[OBJ::WORLD].push_back(add);

	//Bookpile
	add = new AnimationObject("src/meshes/Ghost/BookPile.v", "src/textures/Ghost/books.bmp");
	objects[OBJ::WORLD].push_back(add);

	//BrickWall
	add = new AnimationObject("src/meshes/Ghost/brickwall.v", "src/textures/Ghost/brickwall.bmp");
	objects[OBJ::WORLD].push_back(add);

	//MansionWall
	add = new AnimationObject("src/meshes/Ghost/mansionwall.v", "src/textures/Ghost/mansionwall.bmp");
	objects[OBJ::WORLD].push_back(add);

	//TornWall
	add = new AnimationObject("src/meshes/Ghost/tornwall.v", "src/textures/Ghost/ripped_wall.bmp");
	objects[OBJ::WORLD].push_back(add);

	//MansionFloor
	add = new AnimationObject("src/meshes/Ghost/mansionfloor.v", "src/textures/Ghost/floor.bmp");
	objects[OBJ::WORLD].push_back(add);

	//MansionFlooredge
	add = new AnimationObject("src/meshes/Ghost/mansionflooredge.v", "src/textures/Ghost/floor.bmp");
	objects[OBJ::WORLD].push_back(add);
}

void ObjectManager::loadMonsterObjs()
{
	AnimationObject* add;

	//spikes
	add = new AnimationObject("src/meshes/Enemies/SpikeBase.v", "src/textures/Red.bmp");
	objects[OBJ::ENEMY].push_back(add);

	//bats
	Object* bat1 = new Object("src/meshes/Enemies/BatAngle.v", "src/textures/BatNew.bmp");
	Object* bat2 = new Object("src/meshes/Enemies/BatAngle2.v", "src/textures/BatNew.bmp");
	add = new AnimationObject(bat1, bat2, 0.5f, 0.24f);
	objects[OBJ::ENEMY].push_back(add);

	//flame
	Object* FCube1 = new Object("src/meshes/Enemies/FlameCubeUp.v", "src/textures/Red.bmp");
	Object* FCube2 = new Object("src/meshes/Enemies/FlameCubeDown.v", "src/textures/Red.bmp");
	add = new AnimationObject(FCube1, FCube2, 0.5f, 0.05f);
	objects[OBJ::ENEMY].push_back(add);

	//cube
	Object* slimeC1 = new Object("src/meshes/Enemies/SlimeCubeWobble4.v", "src/textures/black.bmp");
	Object* slimeC2 = new Object("src/meshes/Enemies/SlimeCubeWobble.v", "src/textures/black.bmp");
	add = new AnimationObject(slimeC1, slimeC2, 0.5f, 0.05f);
	objects[OBJ::ENEMY].push_back(add);

	//spider
	Object* spider1 = new Object("src/meshes/Enemies/SpiderWalk3.v", "src/textures/Red.bmp");
	Object* spider2 = new Object("src/meshes/Enemies/SpiderWalk4.v", "src/textures/Red.bmp");
	add = new AnimationObject(spider1, spider2, 0.5f, 0.1f);
	objects[OBJ::ENEMY].push_back(add);

	//ghost
	add = new AnimationObject("src/meshes/Enemies/FlameCubeContracted.v", "src/textures/Red.bmp");
	objects[OBJ::ENEMY].push_back(add);

	//web
	add = new AnimationObject("src/meshes/Interactive Objects/Web.v", "src/textures/Grey.bmp");
	objects[OBJ::ENEMY].push_back(add);

	//spellbook
	add = new AnimationObject("src/meshes/Enemies/FlameCubeExtruded.v", "src/textures/Red.bmp");
	objects[OBJ::ENEMY].push_back(add);

	//Bossbat
	Object* bossBat1 = new Object("src/meshes/Enemies/BatBossBack.v", "src/textures/bossBat.bmp");
	Object* bossBat2 = new Object("src/meshes/Enemies/BatBossFront.v", "src/textures/bossBat.bmp");
	add = new AnimationObject(bossBat1, bossBat2, 0.5f, 0.24f);

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

	//Heart piece
	add = new AnimationObject("src/meshes/Interactive Objects/Heart.v", "src/textures/red.bmp");
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

	//crypt wall
	add = new AnimationObject("src/meshes/Background/cryptwall.v", "src/textures/Background/cryptwalltex.bmp");
	objects[OBJ::BACK].push_back(add);
}

void ObjectManager::update(UpdateAnimCheck* animCheck)
{
	myPlayer->update();
	for (int i = 0; i < WorldID::world_count; i++)
	{

		objects[WORLD][i]->update();
	}
	for (int i = 0; i < EnemyID::enemy_count; i++)
	{
		if (animCheck->enemyUpdate[i])
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
		myPlayer->setWeight(0.5f);
		myPlayer->setDirection(1);
	}
	else if (state == "walk")
	{
		myPlayer->setAnimPoints(1, 2);
		myPlayer->setSpeed(0.2f);
		//myPlayer->setWeight(0.5f);
	}
	/*else if (state == "air")
	{
	myPlayer->setAnimPoints(1, 2);
	myPlayer->setSpeed(0.02f);
	}
	else if (state == "flinch")
	{
	myPlayer->setAnimPoints(2, 2);
	myPlayer->setSpeed(0.02f);
	}*/
	else if (state == "attack")
	{
		myPlayer->setAnimPoints(3, 4);
		myPlayer->setSpeed(0.12f);
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