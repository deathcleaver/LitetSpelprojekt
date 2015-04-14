#include "enemyManager.h"

EnemyManager::EnemyManager()
{
	nrOfEnemies = 0;
}

EnemyManager::EnemyManager(int enemyCount)
{
	nrOfEnemies = enemyCount;
	enemies = new Enemy*[nrOfEnemies];
}

EnemyManager::~EnemyManager()
{
	for (int c = 0; c < nrOfEnemies; c++)
	{
		delete enemies[c];
	}
	delete[]enemies;
}

int EnemyManager::update(float deltaTime)
{
	return 0;
}

int EnemyManager::size()
{
	return nrOfEnemies;
}
