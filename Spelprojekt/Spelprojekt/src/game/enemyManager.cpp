#include "enemyManager.h"
#include "mapChunk.h"
#include "map.h"
#include "Enemies/Spikes.h"
#include "Enemies/Bat.h"
#include "Enemies/Flame.h"
#include "Enemies/Bossbat.h"
#include "Enemies/Bossdummy.h"
#include "Enemies/Cube.h"
#include <sstream>

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{
	for (int c = 0; c < batCount; c++)
	{
		delete bats[c];
	}
	delete[]bats;
	for (int c = 0; c < flameCount; c++)
	{
		delete flames[c];
	}
	delete[]flames;
	for (int c = 0; c < spikeCount; c++)
	{
		delete spikes[c];
	}
	delete[]spikes;
	for (int c = 0; c < cubeCount; c++)
	{
		delete cubes[c];
	}
	delete[]cubes;


	if (boss)
	{
		delete boss;
	}
	if (visitorHolder)
		delete[]visitorHolder;
}

void EnemyManager::init(ifstream &file, int xOffset, int yOffset)
{
	batCount = 0; batMax = 5;
	bats = new Enemy*[batMax];
	spikeCount = 0; spikeMax = 5;
	spikes = new Enemy*[spikeMax];
	flameCount = 0; flameMax = 5;
	flames = new Enemy*[flameMax];
	cubeCount = 0; cubeMax = 5;
	cubes = new Enemy*[cubeMax];

	boss = 0;
	string line;
	string type; //To store type
	glm::vec2 pos; //To store pos

	//Find all enemies, create them, and load them
	
	getline(file, line);
	istringstream iss(line);
	string sub;
	iss >> sub;
	if (atoi(sub.c_str()) == 1) //THERE IS A BOSS HOLY HELL
	{
		iss >> sub;
		type = sub; //Läs bosstyp
		iss >> sub;
		pos.x = atof(sub.c_str());
		pos.x = pos.x + xOffset * 35;
		iss >> sub;
		pos.y = atof(sub.c_str());
		pos.y = pos.y - yOffset * 35;
		addBoss(type, pos);
	}
	getline(file, line);
	iss = istringstream(line);
	iss >> sub;
	int nrOfEnemies = atoi(sub.c_str());
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (!(getline(file, line))) break;
		iss = istringstream(line);
		iss >> sub;
		type = sub; //Läs fiendetyp
		iss >> sub;
		pos.x = atof(sub.c_str());
		pos.x = pos.x + xOffset * 35;
		iss >> sub;
		pos.y = atof(sub.c_str());
		pos.y = pos.y - yOffset * 35;
		addEnemy(type, pos);
	}
}

void EnemyManager::save(ofstream* out, int xOffset, int yOffset)
{
	//reset all enemies to their original pos
	resetEnemies();

	//save boss  OBSERVERA!!!!! 0 boss
	if (boss)
	{
		glm::vec2 bossPos = boss->plsGiveBossPos();
		*out << 1 << " " << boss->getType() << " " << bossPos.x - xOffset * 35 << " " << bossPos.y + yOffset * 35 << endl;
	}
	else
		*out << 0 << " : ChuckTesta" << endl;
	*out << batCount + flameCount + spikeCount  + cubeCount << " : Enemy Count" << endl;

	for (int n = 0; n < batCount; n++)
		*out << "Bat " << bats[n]->readPos().x - xOffset * 35 << " " << bats[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < flameCount; n++)
		*out << "Flame " << flames[n]->readPos().x - xOffset * 35 << " " << flames[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < spikeCount; n++)
		*out << "Spikes " << spikes[n]->readPos().x - xOffset * 35 << " " << spikes[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < cubeCount; n++)
		*out << "Cube " << cubes[n]->readPos().x - xOffset * 35 << " " << cubes[n]->readPos().y + yOffset * 35 << endl;
	
}

void EnemyManager::initEmpty()
{
	flameCount = -1;
	flames = 0;
	spikeCount = -1;
	spikes = 0;
	batCount = -1;
	bats = 0;
}

int EnemyManager::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos, Map* map)
{
	int idX = -1, idY = -1;
	if (visitorHolder)
	{
		for (int c = 0; c < visitorsToSendOut; c++)
			delete visitorHolder[c];
		delete[]visitorHolder;
	}
	visitorsToSendOut = 0;
	int maxVisitors = 5;
	visitorHolder = new Enemy*[maxVisitors];

	int msg = 0;
	glm::vec3 chunkMid = glm::vec3(chunk->xOffset * 35, chunk->yOffset * -35, 0);
	glm::vec3 pos;
	for (int c = 0; c < batCount; c++)
	{
		if (bats[c]->isAlive())
		{
			msg = bats[c]->update(deltaTime, map, playerPos);
			pos = bats[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Bat* visitBat = new Bat((Bat*)bats[c]);
				visitBat->setVisitor();
				bats[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitBat;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < flameCount; c++)
	{
		if (flames[c]->isAlive())
		{
			msg = flames[c]->update(deltaTime, map, playerPos);
			pos = flames[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Flame* visitFlame = new Flame((Flame*)flames[c]);
				visitFlame->setVisitor();
				flames[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitFlame;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < cubeCount; c++)
	{
		if (cubes[c]->isAlive())
		{
			msg = cubes[c]->update(deltaTime, map, playerPos);
			pos = cubes[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Cube* visitCube = new Cube((Cube*)cubes[c]);
				visitCube->setVisitor();
				cubes[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitCube;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	if (boss)
	{
		if (boss->isAlive())
		{
			msg = boss->update(deltaTime, map, playerPos);
		}
	}
	return visitorsToSendOut;
}

int EnemyManager::size(string type)
{
	if (type == "Spikes")
		return spikeCount;
	if (type == "Bat")
		return batCount;
	if (type == "Flame")
		return flameCount;
	if (type == "Cube")
		return cubeCount;
	return 0;
}

void EnemyManager::addEnemy(string type, glm::vec2 pos)
{
	if (type == "Spikes")
	{
		spikes[spikeCount] = new Spikes(pos);
		spikeCount++;
		if (spikeCount == spikeMax)
			expandEnemyArray(spikes, spikeMax);
	}
	if (type == "Bat")
	{
		bats[batCount] = new Bat(pos);
		batCount++;
		if (batCount == batMax)
			expandEnemyArray(bats, batMax);
	}
	if (type == "Flame")
	{
		flames[flameCount] = new Flame(pos);
		flameCount++;
		if (flameCount == flameMax)
			expandEnemyArray(flames, flameMax);
	}
	if (type == "Cube")
	{
		cubes[cubeCount] = new Cube(pos);
		cubeCount++;
		if (cubeCount == cubeMax)
			expandEnemyArray(cubes, cubeMax);
	}
}

int EnemyManager::bindEnemy(int index, GLuint* shader, GLuint* uniform, string type)
{
	if (index == -1)
	{
		if (boss)
			return boss->bindWorldMat(shader, uniform);
	}
	else if (type == "Bat")
		return bats[index]->bindWorldMat(shader, uniform);
	else if (type == "Flame")
		return flames[index]->bindWorldMat(shader, uniform);
	else if (type == "Spikes")
		return spikes[index]->bindWorldMat(shader, uniform);
	else //if (type == "Cube")
		return cubes[index]->bindWorldMat(shader, uniform);
}

Enemy** EnemyManager::getEnemies(string type)
{
	if (type == "Spikes")
		return spikes;
	if (type == "Bat")
		return bats;
	if (type == "Flame")
		return flames;
	if (type == "Cube")
		return cubes;
	return 0;
}

void EnemyManager::resetEnemies()
{
	for (int n = batCount-1; n >= 0; n--)
	{
		if (bats[n]->isVisitor())
		{
			delete bats[n];
			bats[n] = 0;
			batCount--;
		}
		else
			bats[n]->init();
	}
	for (int n = flameCount-1; n >= 0; n--)
	{
		if (flames[n]->isVisitor())
		{
			delete flames[n];
			flames[n] = 0;
			flameCount--;
		}
		else
			flames[n]->init();
	}
	for (int n = cubeCount - 1; n >= 0; n--)
	{
		if (cubes[n]->isVisitor())
		{
			delete cubes[n];
			cubes[n] = 0;
			cubeCount--;
		}
		else
			cubes[n]->init();
	}
	if (boss)
	{
		if (boss->isAlive())
			boss->init();
	}
}

Enemy* EnemyManager::getBoss()
{
	return boss;
}

void EnemyManager::addBoss(string type, glm::vec2 pos)
{
	if (type == "ChuckTesta")
	{
		boss = new Dummy(pos);
		boss->scaleFactor(0.01, 0.01, 0.01);
	}
	else if (type == "Bossbat")
	{
		boss = new Bossbat(pos);
		boss->scaleFactor(2, 2, 2);
	}
}

void EnemyManager::startBoss()
{
	boss->init();
}

void EnemyManager::expandEnemyArray(Enemy**& arr, int &oldMax)
{
	Enemy** temparr = new Enemy*[oldMax + 5];
	for (int c = 0; c < oldMax; c++)
	{
		temparr[c] = arr[c];
	}
	delete[]arr;
	arr = temparr;
	oldMax += 5;
}

void EnemyManager::addOutsider(Enemy* visitor, string type)
{
	if (type == "Bat")
	{
		bats[batCount] = new Bat((Bat*)visitor);
		batCount++;
		if (batCount == batMax)
			expandEnemyArray(bats, batMax);
	}
	if (type == "Flame")
	{
		flames[flameCount] = new Flame((Flame*)visitor);
		flameCount++;
		if (flameCount == flameMax)
			expandEnemyArray(flames, flameMax);
	}
	if (type == "Cube")
	{
		cubes[cubeCount] = new Cube((Cube*)visitor);
		cubeCount++;
		if (cubeCount == cubeMax)
			expandEnemyArray(cubes, cubeMax);
	}
}