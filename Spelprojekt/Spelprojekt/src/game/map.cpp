#include "map.h"

Map::~Map()
{
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
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			msg = chunks[x][y].update(deltaTime);
		}
	}
	return 0;
}

void Map::getChunkIndex(glm::vec2 pos, int* idX, int* idY)
{
	*idX = int(pos.x + 17.5f) / 35;
	*idY = int(pos.y - 17.5f) / -35;
	if (*idX < 0 || *idY > (width - 1))
		*idX = -1;
	if (*idY < 0 || *idY > (height - 1))
		*idY = -1;
	if (*idX != 0 && *idY != 0)
		int k = 13;
}