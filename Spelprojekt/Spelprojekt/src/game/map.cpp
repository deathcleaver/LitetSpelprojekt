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
