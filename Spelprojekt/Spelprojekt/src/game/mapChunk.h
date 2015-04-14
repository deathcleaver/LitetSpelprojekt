#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "gameObject.h";
#include "rect.h"
#include <string>
#include "enemyManager.h"

using namespace std;
#include "gameObject.h"

class MapChunk
{
private:
	EnemyManager* enemyMan;
public:
	MapChunk(){};
	~MapChunk();

	GameObject* chunkBackground = 0;
	GameObject* worldObjs = 0;
	int countWorldObjs = 0;
	Rect*** worldCollide = 0;
	void init(int x, int y);
	int xOffset, yOffset;

	void translateChunk(float x, float y, float z);
};

#endif