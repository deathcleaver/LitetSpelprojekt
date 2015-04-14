#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <string>
#include <fstream>

#include "enemy.h"

using namespace std;

class EnemyManager
{
private:
	int nrOfEnemies;
	Enemy** enemies;

	void addEnemy(string type, glm::vec2 pos, int c);
public:
	EnemyManager();
	EnemyManager(int enemies);
	~EnemyManager();
	int update(float deltaTime);
	int size();
	void init(ifstream &file);
};

#endif