#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "gameObject.h";
#include "rect.h"
#include <string>
#include <sstream>
#include <fstream>

#include "enemyManager.h"

using namespace std;

class MapChunk
{
private:
	EnemyManager* enemyMan;
	//map collision re-use variables
	int x1, y1, sizeX, sizeY;
public:
	MapChunk(){};
	~MapChunk();

	GameObject* chunkBackground = 0;
	GameObject* worldObjs = 0;
	int countWorldObjs = 0;
	Rect*** worldCollide = 0;
	void init(int x, int y);
	int xOffset, yOffset;

	bool collide(Rect* test);
	void translateChunk(float x, float y, float z);

	int countEnemies();
	int bindEnemy(int index, GLuint* shader, GLuint* uniform);

	int update(float deltaTime);

	bool playerVsEnemies(Rect* playerRect);

	bool enemyLives(int index);
};

#endif