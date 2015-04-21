#include "mapChunk.h"

MapChunk::~MapChunk()
{
	delete chunkBackground;
	delete enemyMan;
	delete[] worldObjs;

	if (shrine)
		delete shrine;
	
	if (nrOfLights > 0)
		delete[]lights;

	if (worldCollide)
	{
		for (int x = 0; x < 35; x++)
		{
			for (int y = 0; y < 35; y++)
			{	
				if (worldCollide[x][y] != NULL)
					delete worldCollide[x][y];
			}
			delete[] worldCollide[x];
		}
		delete[] worldCollide;
	}
}

void MapChunk::init(int xIndex, int yIndex, std::string mapname)
{
	//Build chunk filename
	std::stringstream ss;
	ss << "../Spelprojekt/src/map/" << mapname << "/"
		<< xIndex << "_" << yIndex << ".chunk";
	string fileName = ss.str();

	ifstream in;
	in.open(fileName);
	
	
	xOffset = xIndex;
	yOffset = yIndex;
	

	//TODO:: load objects from file
	if (in.is_open())
	{
		enemyMan = new EnemyManager();
		enemyMan->init(in, xOffset, yOffset);

		chunkBackground = new GameObject();
		chunkBackground->init(0);
		chunkBackground->moveTo(xOffset * 35, yOffset * -35);

		glm::vec3 pos;
		glm::vec3 scale;
		string type;
		string line;
		getline(in, line);
		istringstream iss(line);
		string sub;
		iss >> sub;
		countWorldObjs = atoi(sub.c_str());
		worldObjs = new GameObject[countWorldObjs];
		for (int c = 0; c < countWorldObjs; c++)
		{
			if (!(getline(in, line))) break;
			iss = istringstream(line);
			iss >> sub;
			type = sub; //Läs objekttyp
			iss >> sub;
			pos.x = atof(sub.c_str());
			iss >> sub;
			pos.y = atof(sub.c_str());
			iss >> sub;
			pos.z = atof(sub.c_str());
			iss >> sub;
			scale.x = atof(sub.c_str());
			iss >> sub;
			scale.y = atof(sub.c_str());
			iss >> sub;
			scale.z = atof(sub.c_str());
			if (type == "Box")
			{
				worldObjs[c].init(1); //1 = box
				worldObjs[c].moveTo(xOffset * 35, yOffset * -35);
				worldObjs[c].translate(pos.x, pos.y, pos.z);
				worldObjs[c].scaleFactor(scale.x, scale.y, scale.z);
			}

			if (type == "Shrine")
			{
				worldObjs[c].init(2); //2 = playerBase svart
				worldObjs[c].moveTo(xOffset * 35, yOffset * -35);
				worldObjs[c].translate(pos.x, pos.y, pos.z);
				worldObjs[c].scaleFactor(scale.x, scale.y, scale.z);
				shrine = new Shrine(&worldObjs[c]);
			}
		}
		//Load lights
		getline(in, line);
		iss = istringstream(line);
		iss >> sub;
		nrOfLights = atoi(sub.c_str());
		if (nrOfLights > 0)
		{
			lights = new Light[nrOfLights];
			for (int c = 0; c < nrOfLights; c++)
			{
				getline(in, line);
				iss = istringstream(line);
				iss >> sub;
				lights[c].posX = atof(sub.c_str()) + xOffset*35;
				iss >> sub;
				lights[c].posY = atof(sub.c_str()) - yOffset*35;
				iss >> sub;
				lights[c].posZ = atof(sub.c_str());
				iss >> sub;
				lights[c].r = atof(sub.c_str());
				iss >> sub;
				lights[c].g = atof(sub.c_str());
				iss >> sub;
				lights[c].b = atof(sub.c_str());
				iss >> sub;
				lights[c].intensity = atof(sub.c_str());
				iss >> sub;
				lights[c].distance = atof(sub.c_str());
			}
		}

		worldCollide = new Rect**[35];
		for (int c = 0; c < 35; c++)
		{
			worldCollide[c] = new Rect*[35];
		}

		for (int y = 0; y < 35; y++)
		{
			getline(in, line);
			for (int x = 0; x < 35; x++)
			{
				char lineAt = line.at(x);
				if (lineAt == 'X')
				{
					worldCollide[x][y] = new Rect();
					worldCollide[x][y]->initMapRect(xOffset, yOffset, x, y, 0);
				}
				else
				{
					worldCollide[x][y] = 0;
				}
			}
		}
	}
	else //init 0
	{	
		//default background
		chunkBackground = new GameObject();
		chunkBackground->init(0);
		chunkBackground->moveTo(xOffset * 35, yOffset * -35);
		
		
		
		enemyMan = new EnemyManager;
		enemyMan->initEmpty();

		countWorldObjs = 0;
		worldObjs = new GameObject[countWorldObjs];

		worldCollide = new Rect**[35];
		for (int c = 0; c < 35; c++)
			worldCollide[c] = new Rect*[35];

		for (int y = 0; y < 35; y++)
			for (int x = 0; x < 35; x++)
			{
				if (x > 2 && y == 35)
				{
					worldCollide[x][y] = new Rect();
					worldCollide[x][y]->initMapRect(xOffset, yOffset, x, y, 0);
				}
				else
					worldCollide[x][y] = 0;
			}
	}
	in.close();
}

bool MapChunk::collide(Rect* test, int overFlowX, int overFlowY)
{
	test->readData(&x1, &y1, &sizeX, &sizeY);

	x1 -= (sizeX + 1);
	y1 -= (sizeY + 1);

	x1 = x1 + overFlowX * 35;
	y1 = y1 + overFlowY * 35;

	sizeX = x1 + sizeX + sizeX + 2;
	sizeY = y1 + sizeY + sizeY + 2;

	for (int x = x1; x < sizeX; x++)
	{
		for (int y = y1; y < sizeY; y++)
		{
			if (x < 35 && x > -1 && y > -1 && y < 35) //out of bounds check
			{
				if (worldCollide[x][y] != NULL)
				{
					if (worldCollide[x][y]->intersects(test))
						return true;
				}
			}
		}
	}
	return false;
}

int MapChunk::countEnemies(string type)
{
	return enemyMan->size(type);
}

int MapChunk::bindEnemy(int index, GLuint* shader, GLuint* uniform, string type)
{
	return enemyMan->bindEnemy(index, shader, uniform, type);
}

int MapChunk::update(float deltaTime)
{
	int msg = 0;
	if (enemyMan)
		msg = enemyMan->update(deltaTime, this);
	return 0;
}

void MapChunk::respawnEnemies()
{
	enemyMan->resetEnemies();
}

glm::vec3 MapChunk::playerVsEnemies(Rect* playerRect)
{
	Enemy** enemies = enemyMan->getEnemies("Flame");
	int nrOfEnemies = enemyMan->size("Flame");
	glm::vec3 hit = glm::vec3(0,0,-1);
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->getPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Bat");
	nrOfEnemies = enemyMan->size("Bat");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->getPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Spikes");
	nrOfEnemies = enemyMan->size("Spikes");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->getPos();
			}
		}
	}
	Enemy* boss = enemyMan->getBoss();
	if (boss)
	{
		if (boss->isAlive())
		{
			Rect* bossRect = boss->getRekt();
			if (bossRect)
			{
				if (bossRect->intersects(playerRect))
					return hit = boss->getPos();
			}
		}
	}
	return hit;
}

void MapChunk::attackEnemies(Rect* wpnRect, glm::vec3 playerPos, int damage)
{
	Enemy** enemies = enemyMan->getEnemies("Bat");
	int nrOfEnemies = enemyMan->size("Bat");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(wpnRect))
				{
					if (playerPos.x < enemies[c]->getPos().x)
						enemies[c]->hit(damage, false);
					else
						enemies[c]->hit(damage, true);
				}
			}
		}
	}
	enemies = enemyMan->getEnemies("Flame");
	nrOfEnemies = enemyMan->size("Flame");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(wpnRect))
				{
					if (playerPos.x < enemies[c]->getPos().x)
						enemies[c]->hit(damage, false);
					else
						enemies[c]->hit(damage, true);
				}
			}
		}
	}
	enemies = enemyMan->getEnemies("Spikes");
	nrOfEnemies = enemyMan->size("Spikes");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(wpnRect))
				{
					if (playerPos.x < enemies[c]->getPos().x)
						enemies[c]->hit(damage, false);
					else
						enemies[c]->hit(damage, true);
				}
			}
		}
	}
	Enemy* boss = enemyMan->getBoss();
	if (boss)
	{
		Rect* bossRect = boss->getRekt();
		if (bossRect)
		{
			if (bossRect->intersects(wpnRect))
			{
				if (playerPos.x < boss->getPos().x)
					boss->hit(damage, false);
				else
					boss->hit(damage, true);
			}
		}
	}
}

bool MapChunk::playerVsShrine(Rect* playerRect, Shrine*& currentSpawn)
{
	if (shrine)
	{
		Rect* shrineRect = shrine->getRekt();
		if (playerRect->intersects(shrineRect))
		{
			if (currentSpawn != shrine && shrine != 0)
			{
				printf("Hittade en respawnpunkt!\n");
				currentSpawn = shrine;
			}
			return true;
		}
	}
	return false;
}

bool MapChunk::enemyLives(int index, string type)
{
	if (index == -1)
	{
		if (enemyMan->getBoss())
		{
			if (enemyMan->getBoss()->isAlive())
				return true;
		}
	}
	else
	{
		Enemy** enemies = enemyMan->getEnemies(type);
		if (enemies[index]->isAlive())
			return true;
	}
	return false;
}

bool MapChunk::enemyBlinking(int index, string type)
{
	if (index == -1)
	{
		if (enemyMan->getBoss())
		{
			return enemyMan->getBoss()->isBlinking();
		}
	}
	else
	{
		Enemy** enemies = enemyMan->getEnemies(type);
		return enemies[index]->isBlinking();
	}
}

int MapChunk::hasBoss()
{
	Enemy* boss = enemyMan->getBoss();
	{
		if (boss)
		{
			if (boss->isAlive())
				return 1;
			else
				return 2;
		}
	}
	return 0;
}

Light* MapChunk::getLights(int &lightNr)
{
	lightNr = nrOfLights;
	return lights;
}