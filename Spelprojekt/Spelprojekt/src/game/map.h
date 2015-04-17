#ifndef MAP_H
#define MAP_H

#include "mapChunk.h"

class Map
{
private:
	MapChunk** chunks = 0;
	int width = 0;
	int height = 0;
	int* upDraw;
	int* lastUpDraw;
	int counter = 0;
public:
	Map(){};
	~Map();
	void init();
	int readSquareSize() const;
	int readSizeX() const;
	int readSizeY() const;
	MapChunk** getChunks() const;

	int update(float deltaTime);
	//data ref x = -1, y -1 = outofbounds
	void getChunkIndex(glm::vec3 pos, int* idX, int* idY);
	bool collideMap(Rect* test, glm::vec3 pos);
	glm::vec3 collideEnemies(Rect* test, glm::vec3 pos);
	void collideShrine(Rect* test, glm::vec3 pos, Shrine*& currentSpawn);
	void setUpDraw(glm::vec3 pos);
	int* getUpDraw() const;
	void respawnCheck();
};

#endif