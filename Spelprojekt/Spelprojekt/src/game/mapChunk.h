#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "gameObject.h";
#include "rect.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "enemyManager.h"
#include "Shrine.h"
#include "Light.h"

using namespace std;

class MapChunk
{
private:
	EnemyManager* enemyMan;
	//map collision re-use variables
	int x1, y1, sizeX, sizeY;
	Light* lights;
	int nrOfLights;
	int musicId;

	//for mapmaker UNDO
	int lastRecievedItemWorld = -1;
	GameObject* lastRecievedWorld = 0;
public:
	MapChunk(){};
	~MapChunk();

	GameObject* chunkBackground = 0;
	Shrine* shrine = 0;
	
	int countWorldObjs = 0;

	vector<GameObject*> Box_Objs;

	Rect*** worldCollide = 0;
	void init(int x, int y, std::string mapname);
	int xOffset, yOffset;

	bool collide(Rect* test, int overFlowX = 0, int overFlowY = 0);
	void translateChunk(float x, float y, float z);

	int countEnemies();
	int bindEnemy(int index, GLuint* shader, GLuint* uniform);

	int update(float deltaTime);
	void respawnEnemies();
	glm::vec3 playerVsEnemies(Rect* playerRect);
	bool playerVsShrine(Rect* playerRect, Shrine*& currentSpawn);
	void attackEnemies(Rect* wpnRect, glm::vec3 playerPos, int damage);

	bool enemyLives(int index);
	bool enemyBlinking(int index);

	int hasBoss();

	Light* getLights(int &lightNr);
	int getMusicId();

	string getBossType();

	void recieveWorld(GameObject* item);
	void recieveMonster(GameObject* item, bool visitor);
	void recieveBackObj(GameObject* item);
	void recieveLightObj(GameObject* item);
	void recieveSpecial(GameObject* item);
	void addRekt(int x, int y);
	void removeRekt(int x, int y);
};

#endif