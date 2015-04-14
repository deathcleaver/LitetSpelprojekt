#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "gameObject.h";
#include "rect.h"

class MapChunk
{
private:
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