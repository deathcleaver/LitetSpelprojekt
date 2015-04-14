#include "enemyManager.h"
#include <fstream>

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
}

void EnemyManager::init(int enemyCount, string fileName)
{
	nrOfEnemies = enemyCount;
	enemies = new Enemy*[nrOfEnemies];
	
}

int EnemyManager::update(float deltaTime)
{
	return 0;
}

int EnemyManager::size()
{
	return nrOfEnemies;
}

void EnemyManager::readEnemies(string fileName)
{
	ifstream in;
	in.open(fileName);
	if (in)
	{
		//Find all enemies, create them, and load them
		in.close();
	}
	else
	{

	}
}