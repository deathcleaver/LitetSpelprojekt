#include "mapChunk.h"

MapChunk::~MapChunk()
{
	delete chunkBackground;
	delete[] worldObjs;
}

void MapChunk::init()
{
	//TODO:: load objects from file

	chunkBackground = new GameObject;
	chunkBackground->id = 0;

	size = 1;
	worldObjs = new GameObject[size];

	worldObjs[0].id = 1;
	//worldObjs[0].scaleFactor(5, 1, 2); // 5x1x2 size
	//worldObjs[0].translate(0, 0, 0.5);
	
}