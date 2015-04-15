#ifndef MAP_H
#define MAP_H

#include "mapChunk.h"

class Map
{
private:
	MapChunk** chunks = 0;
	int width = 0;
	int height = 0;
public:
	Map(){};
	~Map();
	void init();
	int readSquareSize() const;
	int readSizeX() const;
	int readSizeY() const;
	MapChunk** getChunks() const;
	//data ref x = -1, y -1 = outofbounds
	void getChunkIndex(glm::vec2 pos, int* idX, int* idY);
};

#endif