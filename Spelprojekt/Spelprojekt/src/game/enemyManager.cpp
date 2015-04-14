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

void EnemyManager::init(ifstream &file)
{
	string line;
	string type;
	glm::vec2 pos;
	stringstream ss;

	//Find all enemies, create them, and load them
	getline(file, line);
	ss << line;
	nrOfEnemies = atoi(ss.str().c_str());
	ss.flush();
	enemies = new Enemy*[nrOfEnemies];
	for (int c = 0; c < nrOfEnemies; c++)
	{
		getline(file, line);
		ss << line;
		type = ss.str();
		ss.flush();
		ss << line;
		pos.x = float(atoi(ss.str().c_str()));
		ss.flush();
		ss << line;
		pos.y = float(atoi(ss.str().c_str()));
		addEnemy(type, pos, c);

		ss.flush();
	}
	
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