#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "enemy.h"

class EnemyManager
{
private:
	int nrOfEnemies;
	Enemy** enemies;
public:
	EnemyManager();
	EnemyManager(int enemies);
	~EnemyManager();
	int update(float deltaTime);
	int size();
};

#endif