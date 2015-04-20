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

	Enemy* boss;
	void addBoss(string type, glm::vec2 pos);
public:
	EnemyManager();
	EnemyManager(int enemies);
	~EnemyManager();
	int update(float deltaTime, MapChunk* chunk);
	int size();
	void init(ifstream &file, int xOffset, int yOffset);
	void initEmpty();
	void resetEnemies();
	int bindEnemy(int index, GLuint* shader, GLuint* uniform);

	Enemy** getEnemies();
	Enemy* getBoss();
};

#endif