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
	for (int c = 0; c < nrOfEnemies; c++)
	{
		delete enemies[c];
	}
	delete[]enemies;

	if (boss)
	{
		delete boss;
	}
}

void EnemyManager::init(ifstream &file, int xOffset, int yOffset)
{
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
	nrOfEnemies = atoi(sub.c_str());
	enemies = new Enemy*[nrOfEnemies];
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
		addEnemy(type, pos, c);
	}
}

void EnemyManager::initEmpty()
{
	nrOfEnemies = -1;
	enemies = 0;
}

int EnemyManager::update(float deltaTime, MapChunk* chunk)
{
	int msg = 0;
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			msg = enemies[c]->update(deltaTime, chunk);
		}
	}
	if (boss)
	{
		if (boss->isAlive())
		{
			msg = boss->update(deltaTime, chunk);
		}
	}
	return 0;
}

int EnemyManager::size()
{
	return nrOfEnemies;
}

void EnemyManager::addEnemy(string type, glm::vec2 pos, int c)
{
	if (type == "Spikes")
	{
		enemies[c] = new Spikes(pos);
	}
	if (type == "Bat")
	{
		enemies[c] = new Bat(pos);
	}
	if (type == "Flame")
	{
		enemies[c] = new Flame(pos);
	}
}

int EnemyManager::bindEnemy(int index, GLuint* shader, GLuint* uniform)
{
	if (index == -1)
	{
		if (boss)
			return boss->bindWorldMat(shader, uniform);
	}
	return enemies[index]->bindWorldMat(shader, uniform);
}

Enemy** EnemyManager::getEnemies()
{
	return enemies;
}

void EnemyManager::resetEnemies()
{
	for (int n = 0; n < nrOfEnemies; n++)
	{
		enemies[n]->init();
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