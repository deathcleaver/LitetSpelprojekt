#include "mapChunk.h"

MapChunk::~MapChunk()
{
	delete chunkBackground;
	delete enemyMan;
	delete[] worldObjs;
}

void MapChunk::init(int xIndex, int yIndex)
{
	ifstream in;
	//in.open("xIndex_yIndex");
	enemyMan = new EnemyManager();
	enemyMan->init(in);

	//TODO:: load objects from file

	chunkBackground = new GameObject();
	chunkBackground->init(0);

	size = 1;
	worldObjs = new GameObject[size];

	worldObjs[0].init(1);
	worldObjs[0].scaleFactor(5, 1, 2); // 5x1x2 size
	worldObjs[0].translate(0, 0, 0.5);
	//in.close();
}