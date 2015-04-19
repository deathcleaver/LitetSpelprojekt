#include "map.h"

Map::~Map()
{
	delete[] upDraw;
	delete[] lastUpDraw;
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
	height = 3;
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
	lastUpDraw = new int[9];
	upDraw[0] = 0;
	lastUpDraw[0] = 0;
	for (int n = 1; n < 9; n++)
	{
		upDraw[n] = -1;
		lastUpDraw[n] = -1;
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
	//Chunk respawn check every 10th frame
	if (counter == 0)
	{
		counter = 10;
		respawnCheck();
	}
	counter--;

	//Culled uppdates
	int msg = 0;
	for (int n = 0; n < upDraw[0]; n++)
	{
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)
				msg = chunks[upDraw[x]][upDraw[y]].update(deltaTime);
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
	//add middle
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
	
	int X = 0;
	if (pos.x < Xbounds) // - X
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] - 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2];
		upDraw[0]++;
		X = -1;
	}
	else if (pos.x > 35 - Xbounds) // X
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] + 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2];
		upDraw[0]++;
		X = 1;
	}

	int Y = 0;
	if (pos.y > -Ybounds) // - Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1];
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] - 1;
		upDraw[0]++;
		Y = -1;
	}
	else if (pos.y < -(35 - Ybounds)) // Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1];
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] + 1;
		upDraw[0]++;
		Y = 1;
	}
	
	//diagonals
	if (X == -1 && Y == -1)
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] - 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] - 1;
		upDraw[0]++;
	}
	else if (X == 1 && Y == -1)
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] + 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] - 1;
		upDraw[0]++;
	}
	else if (X == -1 && Y == 1)
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] - 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] + 1;
		upDraw[0]++;
	}
	else if (X == 1 && Y == 1)
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] + 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] + 1;
		upDraw[0]++;
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

glm::vec3 Map::collideEnemies(Rect* test, glm::vec3 pos)
{
	int idX, idY;
	glm::vec3 result;
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

void Map::respawnCheck()
{
	for (int n = 0; n < upDraw[0]; n++)
	{
		int x = n * 2 + 1;
		int y = x + 1;

		//search
		bool found = false;
		for (int k = 0; k < lastUpDraw[0]; k++)
		{
			if (upDraw[x] == lastUpDraw[k * 2 + 1]
				&& upDraw[y] == lastUpDraw[k * 2 + 2])
			{
				found = true;
				break;
			}
		}
		if (!found)
			if (upDraw[x] > -1 && upDraw[x] < width && upDraw[y] > -1 && upDraw[y] < height)
				chunks[upDraw[x]][upDraw[y]].respawnEnemies();
	}
	for (int n = 0; n < 9; n++)
	{
		lastUpDraw[n] = upDraw[n];
	}
}

void Map::playerDiedSoRespawnEnemies(glm::vec3 playerPos)
{
	for (int idX = 0; idX < width; idX++)
	{
		for (int idY = 0; idY < height; idY++)
		{
			chunks[idX][idY].respawnEnemies();
		}
	}
}