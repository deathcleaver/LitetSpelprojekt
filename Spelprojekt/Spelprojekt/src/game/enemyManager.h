#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <string>
#include "enemy.h"

using namespace std;

class EnemyManager
{
private:
	int nrOfEnemies;
	Enemy** enemies;

	void readEnemies(string fileName);
public:
	EnemyManager();
	EnemyManager(int enemies);
	~EnemyManager();
	int update(float deltaTime);
	int size();
	void init(int enemyCount, string fileName);
};

#endif