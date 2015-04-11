#include "map.h"

Map::~Map()
{
	delete[] chunks;
}

void Map::init()
{
	width = 1;
	height = 1;
	chunks = new MapChunk[readSquareSize()];
	for (int n = 0; n < readSquareSize(); n++)
	{
		chunks[n].init();
	}
}

int Map::readSquareSize() const
{ 
	return (width * height); 
}

const MapChunk* Map::getChunks() const
{
	return chunks;
}

