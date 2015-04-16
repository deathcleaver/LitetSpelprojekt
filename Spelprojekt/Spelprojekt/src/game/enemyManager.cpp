#include "enemyManager.h"
#include "Enemies/Spikes.h"
#include "Enemies/Bat.h"
#include "Enemies/Flame.h"
#include <sstream>

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	for (int c = 0; c < nrOfEnemies; c++)
	{
		delete enemies[c];
	}
	delete[]enemies;
}

void EnemyManager::init(ifstream &file, int xOffset, int yOffset)
{
	string line;
	string type; //To store type
	glm::vec2 pos; //To store pos

	//Find all enemies, create them, and load them
	
	getline(file, line);
	istringstream iss(line);
	string sub;
	iss >> sub;
	nrOfEnemies = atoi(sub.c_str());
	enemies = new Enemy*[nrOfEnemies];
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (!(getline(file, line))) break;
		iss = istringstream(line);
		iss >> sub;
		type = sub; //Läs fiendetyp
		iss >> sub;
		pos.x = atof(sub.c_str());
		pos.x = pos.x + xOffset*35;
		iss >> sub;
		pos.y = atof(sub.c_str());
		pos.y = pos.y - yOffset*35;
		addEnemy(type, pos, c);
	}
}

int EnemyManager::update(float deltaTime, MapChunk* chunk)
{
	int msg = 0;
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			msg = enemies[c]->update(deltaTime, chunk);
		}
	}
	return 0;
}

int EnemyManager::size()
{
	return nrOfEnemies;
}

void EnemyManager::addEnemy(string type, glm::vec2 pos, int c)
{
	if (type == "Spikes")
	{
		enemies[c] = new Spikes(pos);
	}
	if (type == "Bat")
	{
		enemies[c] = new Bat(pos);
	}
	if (type == "Flame")
	{
		enemies[c] = new Flame(pos);
	}
}

int EnemyManager::bindEnemy(int index, GLuint* shader, GLuint* uniform)
{
	return enemies[index]->bindWorldMat(shader, uniform);
}

Enemy** EnemyManager::getEnemies()
{
	return enemies;
}