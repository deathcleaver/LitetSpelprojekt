#include "map.h"

Map::~Map()
{
	delete[] upDraw;
	if (chunks)
	{
		for (int x = 0; x < width; x++)
		{
			delete[] chunks[x];
		}
		delete[] chunks;
	}
}

void Map::init()
{
	width = 3;
	height = 1;
	chunks = new MapChunk*[width];
	for (int x = 0; x < width; x++)
	{
		chunks[x] = new MapChunk[height];
		for (int y = 0; y < height; y++)
		{
			chunks[x][y].init(x, y);
		}
	}
	upDraw = new int[9];
	upDraw[0] = 0;
	for (int n = 1; n < 9; n++)
	{
		upDraw[n] = -1;
	}
}

int Map::readSquareSize() const
{ 
	return (width * height); 
}

MapChunk** Map::getChunks() const
{
	return chunks;
}

int Map::readSizeX() const
{
	return width;
}

int Map::readSizeY() const
{
	return height;
}

int Map::update(float deltaTime)
{
	int msg = 0;
	for (int n = 0; n < upDraw[0]; n++)
	{
		if (upDraw[n * 2 + 1] > -1 && upDraw[n * 2 + 1] < width)
			if (upDraw[n * 2 + 2] > -1 && upDraw[n * 2 + 2] < height)
				msg = chunks[upDraw[n * 2 + 1]][upDraw[n * 2 + 2]].update(deltaTime);
	}
	
	return 0;
}

void Map::getChunkIndex(glm::vec3 pos, int* idX, int* idY)
{
	*idX = int(pos.x + 17.5f) / 35;
	*idY = int(pos.y - 17.5f) / -35;

	if (*idX < 0 || *idX > (width - 1))
		*idX = -1;
	if (*idY < 0 || *idY > (height - 1))
		*idY = -1;
}

void Map::setUpDraw(glm::vec3 pos)
{
	upDraw[0] = 0;
	getChunkIndex(pos, &upDraw[1], &upDraw[2]);
	if (upDraw[1] == -1 || upDraw[2] == -1)
		return;

	upDraw[0]++;

	pos.x = int(pos.x + 17.5f) % 35;
	pos.y = int(pos.y - 17.5f) % 35;
	//how far from edges the map will cull.
	//numbers bigger than 17 will crash the game
	int Xbounds = 17;
	int Ybounds = 10;

	if (pos.x < Xbounds) //DOUBLE -X DRAW
	{
		upDraw[0]++;
		upDraw[3] = upDraw[1] - 1;
		upDraw[4] = upDraw[2];
		
		//if QUAD DRAW -X Y
		if (pos.y < -35 + Ybounds)
		{
			upDraw[0]++;
			upDraw[5] = upDraw[1] - 1;
			upDraw[6] = upDraw[2] + 1;

			upDraw[0]++;
			upDraw[7] = upDraw[1];
			upDraw[8] = upDraw[2] + 1;
		}
		//if QUAD DRAW -X -Y
		else if (pos.y < -Ybounds)
		{
			upDraw[0]++;
			upDraw[5] = upDraw[1] - 1;
			upDraw[6] = upDraw[2] - 1;

			upDraw[0]++;
			upDraw[7] = upDraw[1];
			upDraw[8] = upDraw[2] - 1;
		}
	}
	if (pos.x > 35 - Xbounds) //DOUBLE X DRAW
	{
		upDraw[0]++;
		upDraw[3] = upDraw[1] + 1;
		upDraw[4] = upDraw[2];

		//if QUAD DRAW X Y
		if (pos.y < -35 + Ybounds)
		{
			upDraw[0]++;
			upDraw[5] = upDraw[1] + 1;
			upDraw[6] = upDraw[2] + 1;

			upDraw[0]++;
			upDraw[7] = upDraw[1];
			upDraw[8] = upDraw[2] + 1;
		}
		//if QUAD DRAW X -Y
		else if (pos.y > -Ybounds)
		{
			upDraw[0]++;
			upDraw[5] = upDraw[1] + 1;
			upDraw[6] = upDraw[2] + 1;

			upDraw[0]++;
			upDraw[7] = upDraw[1];
			upDraw[8] = upDraw[2] - 1;
		}
	}
	else //DOUBLE Y DRAW
	{
		if (pos.y < -35 + Ybounds)
		{
			upDraw[0]++;
			upDraw[3] = upDraw[1];
			upDraw[4] = upDraw[2] + 1;
		}
		//DOUBLE -Y DRAW
		else if (pos.y > -Ybounds)
		{
			upDraw[0]++;
			upDraw[3] = upDraw[1];
			upDraw[4] = upDraw[2] - 1;
		}
	}
}

int* Map::getUpDraw() const
{
	return upDraw;
}

bool Map::collideMap(Rect* test, glm::vec3 pos)
{
	int idX, idY;
	bool result = false;
	getChunkIndex(pos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		result = chunks[idX][idY].collide(test);
		if (result)
			return result;

		//on edge of chunk?
		int indexX, indexY, sizeX, sizeY;
		test->readData(&indexX, &indexY, &sizeX, &sizeY);

		if (indexX - sizeX < 1 && idX > 0) //need extra check -X
		{
			result = chunks[idX - 1][idY].collide(test, 1);
			if (result)
				return result;}

		if (indexX + sizeX > 33 && idX < width-1) //need extra check + X
		{
			result = chunks[idX + 1][idY].collide(test, -1);
			if (result)
				return result;}

		if (indexY - sizeY < 1 && idY > 0) //need extra check - Y
		{
			result = chunks[idX][idY-1].collide(test, 0, 1);
			if (result)
				return result;}

		if (indexY + sizeY > 33 && idY < height-1) //need extra check + Y
		{
			result = chunks[idX ][idY+1].collide(test, 0 -1);
			if (result)
				return result;}
	}
	return result;
}


bool Map::collideEnemies(Rect* test, glm::vec3 pos)
{
	int idX, idY;
	bool result = false;
	getChunkIndex(pos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		result = chunks[idX][idY].playerVsEnemies(test);
	}
	return result;
}

void Map::collideShrine(Rect* test, glm::vec3 pos, Shrine*& currentSpawn)
{
	int idX, idY;
	getChunkIndex(pos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		chunks[idX][idY].playerVsShrine(test, currentSpawn);
	}
}