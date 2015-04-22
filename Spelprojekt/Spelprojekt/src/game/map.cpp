#include "map.h"

Map::~Map()
{
	delete[] upDraw;
	delete[] lastUpDraw;
	if (chunks)
	{
		for (int x = 0; x < width; x++)
		{
			delete[] chunks[x];
		}
		delete[] chunks;
	}
}

void Map::init()
{
	if (chunks)
	{
		for (int x = 0; x < width; x++)
		{
			delete[] chunks[x];
		}
		delete[] chunks;
	}
	std::string fileName = "../Spelprojekt/src/map/maps";
	std::string line;
	std::string fetch;
	ifstream in;
	in.open(fileName);

	std::getline(in, line);
	stringstream iss(line);
	iss >> fetch;
	int mapcount = atoi(fetch.c_str());

	std::getline(in, line);
	iss = stringstream(line); //reset
	std::getline(in, line);
	iss << line;
	iss >> fetch;
	fileName = fetch;
	iss >> fetch;
	width = atoi(fetch.c_str());
	iss >> fetch;
	height = atoi(fetch.c_str());

	chunks = new MapChunk*[width];
	for (int x = 0; x < width; x++)
	{
		chunks[x] = new MapChunk[height];
		for (int y = 0; y < height; y++)
		{
			chunks[x][y].init(x, y, fileName);
		}
	}
	upDraw = new int[9];
	lastUpDraw = new int[9];
	upDraw[0] = 0;
	lastUpDraw[0] = 0;
	for (int n = 1; n < 9; n++)
	{
		upDraw[n] = -1;
		lastUpDraw[n] = -1;
	}
}

int Map::readSquareSize() const
{ 
	return (width * height); 
}

MapChunk** Map::getChunks() const
{
	return chunks;
}

int Map::readSizeX() const
{
	return width;
}

int Map::readSizeY() const
{
	return height;
}

int Map::update(float deltaTime, glm::vec3 playerPos)
{
	//Chunk respawn check every 10th frame
	if (counter == 0)
	{
		counter = 10;
		respawnCheck();
	}
	counter--;

	//Culled uppdates
	int msg = 0;
	for (int n = 0; n < upDraw[0]; n++)
	{
		int x = n * 2 + 1;
		int y = x + 1;
		if (upDraw[x] > -1 && upDraw[x] < width)
			if (upDraw[y] > -1 && upDraw[y] < height)
				msg = chunks[upDraw[x]][upDraw[y]].update(deltaTime, playerPos, this);
	}
	
	int idX, idY;
	getChunkIndex(playerPos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		msg = chunks[idX][idY].hasBoss(); //0 = no boss, 1 = alive boss, 2 = dead boss
		
	}
	else
	{
		return 5; //Player out-of-map death
	}

	return msg;
}

glm::vec3 Map::getChunkMiddle(glm::vec3 playerpos)
{
	int idX, idY;
	getChunkIndex(playerpos, &idX, &idY);
	float x = 0, y = 0;
	x = idX * 35;
	y = -idY * 35;
	return glm::vec3(x, y, 0.0f);
}

void Map::getChunkIndex(glm::vec3 pos, int* idX, int* idY)
{
	*idX = int(pos.x + 17.5f) / 35;
	*idY = int(pos.y - 17.5f) / -35;

	if (*idX < 0 || *idX > (width - 1))
		*idX = -1;
	if (*idY < 0 || *idY > (height - 1))
		*idY = -1;
}

void Map::setUpDraw(glm::vec3 pos)
{
	upDraw[0] = 0;
	//add middle
	getChunkIndex(pos, &upDraw[1], &upDraw[2]);
	if (upDraw[1] == -1 || upDraw[2] == -1)
		return;

	upDraw[0]++;

	pos.x = int(pos.x + 17.5f) % 35;
	pos.y = int(pos.y - 17.5f) % 35;
	//how far from edges the map will cull.
	//numbers bigger than 17 will crash the game
	int Xbounds = 15;
	int Ybounds = 10;
	
	int XboundsD = 13;
	int YboundsD = 8;

	int X = 0;
	if (pos.x < Xbounds) // - X
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] - 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2];
		upDraw[0]++;
		X = -1;
	}
	else if (pos.x > 34 - Xbounds) // X
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] + 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2];
		upDraw[0]++;
		X = 1;
	}

	int Y = 0;
	if (pos.y > -Ybounds) // - Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1];
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] - 1;
		upDraw[0]++;
		Y = -1;
	}
	else if (pos.y < -(34 - Ybounds)) // Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1];
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] + 1;
		upDraw[0]++;
		Y = 1;
	}
	
	//diagonals
	if (pos.x < XboundsD && pos.y > -YboundsD)  // - X  - Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] - 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] - 1;
		upDraw[0]++;
	}
	else if (pos.x > 34 - XboundsD && pos.y > -YboundsD) // X  - Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] + 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] - 1;
		upDraw[0]++;
	}
	else if (pos.x < XboundsD && pos.y < -(34 - YboundsD)) // - X  Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] - 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] + 1;
		upDraw[0]++;
	}
	else if (pos.x > 34 - XboundsD && pos.y < -(34 - YboundsD))  // X  Y
	{
		upDraw[upDraw[0] * 2 + 1] = upDraw[1] + 1;
		upDraw[upDraw[0] * 2 + 2] = upDraw[2] + 1;
		upDraw[0]++;
	}
}

int* Map::getUpDraw() const
{
	return upDraw;
}

bool Map::collideMap(Rect* test, glm::vec3 pos)
{
	int idX, idY;
	bool result = false;
	getChunkIndex(pos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		result = chunks[idX][idY].collide(test);
		if (result)
			return result;

		//on edge of chunk?
		int indexX, indexY, sizeX, sizeY;
		test->readData(&indexX, &indexY, &sizeX, &sizeY);

		if (indexX - sizeX < 1 && idX > 0) //need extra check -X
		{
			result = chunks[idX - 1][idY].collide(test, 1);
			if (result)
				return result;}

		if (indexX + sizeX > 33 && idX < width-1) //need extra check + X
		{
			result = chunks[idX + 1][idY].collide(test, -1);
			if (result)
				return result;}

		if (indexY - sizeY < 1 && idY > 0) //need extra check - Y
		{
			result = chunks[idX][idY-1].collide(test, 0, 1);
			if (result)
				return result;}

		if (indexY + sizeY > 33 && idY < height-1) //need extra check + Y
		{
			result = chunks[idX ][idY+1].collide(test, 0 -1);
			if (result)
				return result;}
	}
	return result;
}

glm::vec3 Map::collideEnemies(Rect* test, glm::vec3 pos)
{
	int idX, idY;
	glm::vec3 result;
	getChunkIndex(pos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		result = chunks[idX][idY].playerVsEnemies(test);
	}
	return result;
}

void Map::attackEnemies(Rect* wpnRect, glm::vec3 playerpos, int damage)
{
	int idX, idY;
	getChunkIndex(playerpos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		chunks[idX][idY].attackEnemies(wpnRect, playerpos, damage);
	}
}

bool Map::collideShrine(Rect* test, glm::vec3 pos, Shrine*& currentSpawn)
{
	int idX, idY;
	getChunkIndex(pos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		return chunks[idX][idY].playerVsShrine(test, currentSpawn);
	}
}

void Map::respawnCheck()
{
	for (int n = 0; n < upDraw[0]; n++)
	{
		int x = n * 2 + 1;
		int y = x + 1;

		//search
		bool found = false;
		for (int k = 0; k < lastUpDraw[0]; k++)
		{
			if (upDraw[x] == lastUpDraw[k * 2 + 1]
				&& upDraw[y] == lastUpDraw[k * 2 + 2])
			{
				found = true;
				break;
			}
		}
		if (!found)
			if (upDraw[x] > -1 && upDraw[x] < width && upDraw[y] > -1 && upDraw[y] < height)
				chunks[upDraw[x]][upDraw[y]].respawnEnemies();
	}
	for (int n = 0; n < 9; n++)
	{
		lastUpDraw[n] = upDraw[n];
	}
}

void Map::playerDiedSoRespawnEnemies()
{
	for (int idX = 0; idX < width; idX++)
	{
		for (int idY = 0; idY < height; idY++)
		{
			chunks[idX][idY].respawnEnemies();
		}
	}
}

std::string Map::getBoss(glm::vec3 playerpos, bool startBoss)
{
	int idX, idY;
	getChunkIndex(playerpos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		std::string boss = chunks[idX][idY].getBossType();
		if (startBoss && boss != "ChuckTesta" && chunks[idX][idY].hasBoss() != 2)
			chunks[idX][idY].initBoss();
		return boss;
	}
	return "No boss, wtf happened";
}

void Map::findNewHome(Enemy* orphan)
{
	glm::vec3 enemyPos = orphan->readPos();
	int idX, idY;
	getChunkIndex(enemyPos, &idX, &idY);
	if (idX != -1 && idY != -1)
	{
		chunks[idX][idY].addVisitor(orphan, orphan->getType());
	}
}