#include "mapChunk.h"

MapChunk::~MapChunk()
{
	delete chunkBackground;
	delete enemyMan;
	delete[] worldObjs;

	if (worldCollide)
	{
		for (int x = 0; x < 35; x++)
		{
			for (int y = 0; y < 35; y++)
			{	
				if (worldCollide[x][y] != 0)
					delete worldCollide[x][y];
			}
			delete[] worldCollide[x];
		}
		delete[] worldCollide;
	}
}

void MapChunk::init(int xIndex, int yIndex)
{
	//Build chunk filename
	std::stringstream ss;
	ss << "../Spelprojekt/src/map/" <<
		xIndex << "_" << yIndex << ".chunk";
	string fileName = ss.str();

	ifstream in;
	in.open(fileName);
	
	enemyMan = new EnemyManager();
	xOffset = xIndex;
	yOffset = yIndex;
	enemyMan->init(in, xOffset, yOffset);

	//TODO:: load objects from file
	
	
	chunkBackground = new GameObject();
	chunkBackground->init(0);
	chunkBackground->moveTo(xOffset * 35, yOffset * -35);
	/*
	countWorldObjs = 1;
	worldObjs = new GameObject[countWorldObjs];

	worldObjs[0].init(1);
	worldObjs[0].scaleFactor(5, 1, 2); // 5x1x2 size
	worldObjs[0].moveTo(xOffset * 35, yOffset * -35); //align to grid
	worldObjs[0].translate(0, 0, 0.5);
	*/
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
	}

	worldCollide = new Rect**[35];
	for (int c = 0; c < 35; c++)
	{
		worldCollide[c] = new Rect*[35];
	}
	
	for (int y = 0; y < 35; y++)
	{
		if (!(getline(in, line))) break;
		
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
			/*
			if (y == 17 && x >= 15 && x <= 19)
			{
				worldCollide[x][y] = new Rect();
				worldCollide[x][y]->initMapRect(xOffset, yOffset, x, y, 0);
			}
			else if (y == 16 && x >= 22 && x <= 24)
			{
				worldCollide[x][y] = new Rect();
				worldCollide[x][y]->initMapRect(xOffset, yOffset, x, y, 0);
			}
			else
				worldCollide[x][y] = 0;
			*/
		}
	}
	in.close();
}

bool MapChunk::collide(Rect* test)
{
	test->readData(&x1, &y1, &sizeX, &sizeY);
	sizeX = sizeX + x1 +1;
	sizeY = sizeY + y1 +1;

	for (int x = x1; x < sizeX; x++)
	{
		for (int y = y1; y < sizeY; y++)
		{
			if (x < 35 && x > 0 && y > 0 && y < 35) //out of bounds check
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

int MapChunk::countEnemies()
{
	return enemyMan->size();
}

int MapChunk::bindEnemy(int index, GLuint* shader, GLuint* uniform)
{
	return enemyMan->bindEnemy(index, shader, uniform);
}

int MapChunk::update(float deltaTime)
{
	int msg = 0;
	if (enemyMan)
		msg = enemyMan->update(deltaTime, this);
	return 0;
}

bool MapChunk::playerVsEnemies(Rect* playerRect)
{
	Enemy** enemies = enemyMan->getEnemies();
	int nrOfEnemies = enemyMan->size();
	bool hit = false;
	for (int c = 0; c < nrOfEnemies && !hit; c++)
	{
		Rect* enemyRect = enemies[c]->getRekt();
		if (enemyRect && enemies[c]->isAlive())
		{
			if (enemyRect->intersects(playerRect))
			{
				hit = true;
				enemies[c]->hit(1);
			}
		}
	}
	return hit;
}

bool MapChunk::enemyLives(int index)
{
	Enemy** enemies = enemyMan->getEnemies();
	if (enemies[index]->isAlive())
		return true;
	return false;
}