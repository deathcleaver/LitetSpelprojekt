#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include <string>
#include "enemyManager.h"

using namespace std;

class MapChunk
{
private:
	EnemyManager* enemyMan;
public:
	MapChunk(){};
	~MapChunk();
	GameObject* chunkBackground;
	GameObject* worldObjs;
	int size = 0;
	void init();
	void translateChunk(float x, float y, float z);
};

#endif