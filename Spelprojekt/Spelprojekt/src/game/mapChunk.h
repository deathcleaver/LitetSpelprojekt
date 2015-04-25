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
#include "../legend.h"

using namespace std;

class Map;

class MapChunk
{
private:
	EnemyManager* enemyMan;
	//map collision re-use variables
	int x1, y1, sizeX, sizeY;
	Light* lights;
	int nrOfLights = 0;
	int musicId = -1;
	int countWorldObjs = 0;
	
	//for mapmaker UNDO
	int lastRecievedItemWorld = -1;
	GameObject* lastRecievedWorld = 0;
	
	void saveObject(GameObject* object, ofstream* out);
	void loadObjectOld(ifstream* in);
	void loadObject(ifstream* in);
public:
	MapChunk(){};
	~MapChunk();

	GameObject* chunkBackground = 0;
	Shrine* shrine = 0;
	
	string mapnamepath = "";

	vector<vector<GameObject*>> gameObjects;

	Rect*** worldCollide = 0;
	void init(int x, int y, std::string mapname);
	void initOld(int x, int y, std::string mapname);
	int xOffset, yOffset;

	bool collide(Rect* test, int overFlowX = 0, int overFlowY = 0);
	void translateChunk(float x, float y, float z);

	int countEnemies(string type);
	int bindEnemy(int index, GLuint* shader, GLuint* uniform, std::string type);

	int update(float deltaTime, glm::vec3 playerPos, Map* map);
	void respawnEnemies();
	glm::vec3 playerVsEnemies(Rect* playerRect);
	bool playerVsShrine(Rect* playerRect, Shrine*& currentSpawn);
	void attackEnemies(Rect* wpnRect, glm::vec3 playerPos, int damage);

	bool enemyLives(int index, string type);
	bool enemyBlinking(int index, string type);

	int hasBoss();

	Light* getFlameLight(int index);
	Light* getLights(int &lightNr);
	int getMusicId();

	void initBoss();
	string getBossType();

	void recieveWorld(GameObject* item);
	GameObject* takeClosestWorldItem(glm::vec3 pos);
	void recieveMonster(GameObject* item);
	void recieveBackObj(GameObject* item);
	void recieveLightObj(GameObject* item);
	void recieveSpecial(GameObject* item);
	void addRekt(int x, int y);
	void removeRekt(int x, int y);
	void addVisitor(Enemy* visitor, string type);
	void saveChunk();
};

#endif