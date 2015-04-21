#ifndef MAP_H
#define MAP_H

#include "mapChunk.h"
#include "enemy.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

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

	int update(float deltaTime, glm::vec3 playerPos);
	//data ref x = -1, y -1 = outofbounds
	void getChunkIndex(glm::vec3 pos, int* idX, int* idY);
	bool collideMap(Rect* test, glm::vec3 pos);
	glm::vec3 collideEnemies(Rect* test, glm::vec3 pos);
	bool collideShrine(Rect* test, glm::vec3 pos, Shrine*& currentSpawn);
	void attackEnemies(Rect* wpnRect, glm::vec3 playerpos, int damage);
	void setUpDraw(glm::vec3 pos);
	int* getUpDraw() const;
	void respawnCheck();
	void playerDiedSoRespawnEnemies();

	glm::vec3 getChunkMiddle(glm::vec3 playerpos);

	std::string getBoss(glm::vec3 playerpos, bool startBoss);

	void findNewHome(Enemy* orphan);
};

#endif