#include "mapChunk.h"
#include "rect.h"

MapChunk::~MapChunk()
{
	delete chunkBackground;
	delete enemyMan;
	delete[] worldObjs;

	if (worldCollide)
	{
		for (int x = 0; x < 35; x++)
		{
			for (int y = 0; y < 35; y++)
			{	
				if (worldCollide[x][y] != NULL)
					delete worldCollide[x][y];
			}
			delete[] worldCollide[x];
		}
		delete[] worldCollide;
	}
}

void MapChunk::init(int xIndex, int yIndex)
{
	ifstream in;
	//in.open("xIndex_yIndex");
	enemyMan = new EnemyManager();
	enemyMan->init(in);

	//TODO:: load objects from file
	xOffset = xIndex;
	yOffset = yIndex;
	chunkBackground = new GameObject();
	chunkBackground->init(0);
	chunkBackground->moveTo(xOffset * 35, yOffset * -35);
	countWorldObjs = 1;
	worldObjs = new GameObject[countWorldObjs];

	worldObjs[0].init(1);
	worldObjs[0].scaleFactor(5, 1, 2); // 5x1x2 size
	worldObjs[0].moveTo(xOffset * 35, yOffset * -35); //align to grid
	worldObjs[0].translate(0, 0, 0.5);

	worldCollide = new Rect**[35];
	for (int x = 0; x < 35; x++)
	{
		worldCollide[x] = new Rect*[35];
		for (int y = 0; y < 35; y++)
		{
			if (y == 17)
			{
				worldCollide[x][y] = new Rect();
				worldCollide[x][y]->initMapRect(xOffset, yOffset, x, y, 0);
			}
			else
				worldCollide[x][y] = 0;
		}
	}
	//in.close();
}