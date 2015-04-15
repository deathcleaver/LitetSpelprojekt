#include "mapChunk.h"

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
	//Build chunk filename
	std::stringstream iss;
	//iss << ""

	ifstream in;
	//in.open("xIndex_yIndex");
	enemyMan = new EnemyManager();
	xOffset = xIndex;
	yOffset = yIndex;
	enemyMan->init(in, xOffset, yOffset);

	//TODO:: load objects from file
	
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

bool MapChunk::collide(Rect* test)
{
	test->readData(&x1, &y1, &sizeX, &sizeY);
	x1 = (x1 + 17) % 35;
	y1 = (y1 - 17) % 35;
	y1 *= -1;
	sizeX = sizeX + 1 + x1;
	sizeY = sizeY + 1 + y1;
	for (x1; x1 < sizeX; x1++)
	{
		for (y1; y1 < sizeY; y1++)
		{
			if (x1 < 35 && x1 > 0 && y1 > 0 && y1 < 35) //out of bounds check
			{
				if (worldCollide[x1][y1] != NULL)
				{
					if (worldCollide[x1][y1]->intersects(test))
						return true;
				}
			}
		}
	}

	return false;
}

int MapChunk::countEnemies()
{
	return enemyMan->size();
}

int MapChunk::bindEnemy(int index, GLuint* shader, GLuint* uniform)
{
	return enemyMan->bindEnemy(index, shader, uniform);
}

int MapChunk::update(float deltaTime)
{
	int msg = 0;
	if (enemyMan)
		msg = enemyMan->update(deltaTime);
	return 0;
}