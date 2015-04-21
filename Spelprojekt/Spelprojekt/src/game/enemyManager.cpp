#include "enemyManager.h"
#include "Enemies/Spikes.h"
#include "Enemies/Bat.h"
#include "Enemies/Flame.h"
#include "Enemies/Bossbat.h"
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

	if (boss)
	{
		delete boss;
	}
}

void EnemyManager::init(ifstream &file, int xOffset, int yOffset)
{
	batCount = 0; batMax = 5;
	bats = new Enemy*[batMax];
	spikeCount = 0; spikeMax = 5;
	spikes = new Enemy*[spikeMax];
	flameCount = 0; flameMax = 5;
	flames = new Enemy*[flameMax];

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
		getline(file, line);
		iss = istringstream(line);
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

void EnemyManager::initEmpty()
{
	flameCount = -1;
	flames = 0;
	spikeCount = -1;
	spikes = 0;
	batCount = -1;
	bats = 0;
}

int EnemyManager::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos)
{
	int msg = 0;
	for (int c = 0; c < batCount; c++)
	{
		if (bats[c]->isAlive())
		{
			msg = bats[c]->update(deltaTime, chunk, playerPos);
		}
	}
	for (int c = 0; c < flameCount; c++)
	{
		if (flames[c]->isAlive())
		{
			msg = flames[c]->update(deltaTime, chunk, playerPos);
		}
	}
	if (boss)
	{
		if (boss->isAlive())
		{
			msg = boss->update(deltaTime, chunk, playerPos);
		}
	}
	return 0;
}

int EnemyManager::size(string type)
{
	if (type == "Spikes")
	{
		return spikeCount;
	}
	if (type == "Bat")
	{
		return batCount;
	}
	if (type == "Flame")
	{
		return flameCount;
	}
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
	else //if (type == "Spikes")
		return spikes[index]->bindWorldMat(shader, uniform);
}

Enemy** EnemyManager::getEnemies(string type)
{
	if (type == "Spikes")
	{
		return spikes;
	}
	if (type == "Bat")
	{
		return bats;
	}
	if (type == "Flame")
	{
		return flames;
	}
	return 0;
}

void EnemyManager::resetEnemies()
{
	for (int n = batCount-1; n >= 0; n--)
	{
		if (bats[n]->isVisitor())
		{
			delete bats[n];
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
			flameCount--;
		}
		else
			flames[n]->init();
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
	if (type == "Fake")
	{
		boss = new Spikes(pos);
		boss->scaleFactor(0.01, 0.01, 0.01);
	}
	if (type == "Bossbat")
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
	//	flames[flameCount] = new Flame(visitor);
	//	flameCount++;
	//	if (flameCount == flameMax)
	//		expandEnemyArray(flames, flameMax);
	}
}