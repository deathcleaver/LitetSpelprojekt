#include "enemyManager.h"
#include "Enemies/Spikes.h"
#include "Enemies/Bat.h"
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
	string type = "Spikes";
	glm::vec2 pos = glm::vec2(-1+xOffset*35,-1+yOffset*35);

	nrOfEnemies = 1;
	enemies = new Enemy*[nrOfEnemies];
	addEnemy(type, pos, 0);
	

	//Find all enemies, create them, and load them
	/*
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
		pos.x = atoi(sub.c_str());
		iss >> sub;
		pos.y = atoi(sub.c_str());
		addEnemy(type, pos, c);
	}
	*/
}

int EnemyManager::update(float deltaTime)
{
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
		//enemies[c] = new Flame(pos);
	}
}

int EnemyManager::bindEnemy(int index, GLuint* shader, GLuint* uniform)
{
	return enemies[index]->bindWorldMat(shader, uniform);
}