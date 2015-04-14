#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "gameObject.h"

class MapChunk
{
private:
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