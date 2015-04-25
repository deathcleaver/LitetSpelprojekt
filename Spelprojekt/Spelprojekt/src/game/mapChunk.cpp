#include "mapChunk.h"
#include "map.h"
#include "Enemies/Flame.h"

MapChunk::~MapChunk()
{
	saveChunk();

	delete chunkBackground;
	delete enemyMan;

	if (shrine)
		delete shrine;
	
	if (nrOfLights > 0)
		delete[]lights;

	for (int n = 0; n < WorldID::world_count; n++)
	{
		for (int k = 0; k < gameObjects[n].size(); k++)
		{
			delete  gameObjects[n][k];
		}
	}

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
	gameObjects = vector<vector<GameObject*>>();
	
	for (int n = 0; n < WorldID::world_count; n++)
	{
		vector<GameObject*> temp;
		gameObjects.push_back(temp);
	}

	//Build chunk filename
	std::stringstream ss;
	ss << "../Spelprojekt/src/map/" << mapname << "/"
		<< xIndex << "_" << yIndex << ".chunk";
	string fileName = ss.str();

	ifstream in;
	in.open(fileName);
	
	
	xOffset = xIndex;
	yOffset = yIndex;
	
	mapnamepath = mapname;

	//load objects from file
	if (in.is_open())
	{
		string line;
		string sub;
		stringstream ss;

		// --- Load Monsters --- 
		enemyMan = new EnemyManager();
		enemyMan->init(in, xOffset, yOffset);

		// --- Load Background ---
		getline(in, line);
		ss = stringstream(line);
		int id;
		ss >> sub;
		id = atoi(sub.c_str());
		if (id == -1) //no background
			chunkBackground = 0;
		else
		{
			glm::vec3 pos;
			ss >> sub;
			pos.x = atof(sub.c_str());
			ss >> sub;
			pos.y = atof(sub.c_str());
			ss >> sub;
			pos.z = atof(sub.c_str());
			chunkBackground = new GameObject();
			chunkBackground->init(id);
			chunkBackground->moveTo(pos);
			chunkBackground->translate(xOffset * 35, yOffset * -35, 0);
		}

		// --- Load Shrine ---
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		id = atoi(sub.c_str());
		shrine = 0;
		if (id > 0) //if shrine
		{
			glm::vec3 pos;
			ss >> sub;
			pos.x = atof(sub.c_str());
			ss >> sub;
			pos.y = atof(sub.c_str());
			ss >> sub;
			pos.z = atof(sub.c_str());
			GameObject* temp = new GameObject();
			temp->init(MiscID::shrine);
			temp->moveTo(pos);
			temp->translate(xOffset * 35, yOffset * -35, 0);
			shrine = new Shrine(temp, MiscID(id));
		}

		// --- Load World Count --- 
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		int countWorldObjs = atoi(sub.c_str());
		
		// --- Load World Objects ---
		for (int n = 0; n < countWorldObjs; n++)
		{
			loadObject(&in);
		}

		// --- Load music id --- 
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		musicId = atoi(sub.c_str());

		// --- Load Light count ---
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		nrOfLights = atoi(sub.c_str());

		//--- Load Lights --- 
		if (nrOfLights > 0 || shrine)
		{
			if (shrine)
				lights = new Light[nrOfLights+1];
			else
				lights = new Light[nrOfLights];
			for (int c = 0; c < nrOfLights; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				lights[c].posX = atof(sub.c_str()) + xOffset*35;
				ss >> sub;
				lights[c].posY = atof(sub.c_str()) - yOffset*35;
				ss >> sub;
				lights[c].posZ = atof(sub.c_str());
				ss >> sub;
				lights[c].r = atof(sub.c_str());
				ss >> sub;
				lights[c].g = atof(sub.c_str());
				ss >> sub;
				lights[c].b = atof(sub.c_str());
				ss >> sub;
				lights[c].intensity = atof(sub.c_str());
				ss >> sub;
				lights[c].distance = atof(sub.c_str());
				ss >> sub;
				lights[c].volume = atoi(sub.c_str());
			}
			if (shrine)
			{
				nrOfLights++;
				glm::vec3 lPos = shrine->returnRune()->readPos();
				MiscID runeType = shrine->getRune();
				shrine->resetRune();
				lights[nrOfLights - 1].posX = lPos.x;
				lights[nrOfLights - 1].posY = lPos.y;
				lights[nrOfLights - 1].posZ = lPos.z+0.5;

				switch (runeType)
				{
				case MiscID::rune_range: //flame
					lights[nrOfLights - 1].r = 0.5;
					lights[nrOfLights - 1].g = 0.25;
					lights[nrOfLights - 1].b = 0.01;
					break;
				case MiscID::rune_damage: //spark
					lights[nrOfLights - 1].r = 0.0;
					lights[nrOfLights - 1].g = 0.5;
					lights[nrOfLights - 1].b = 0.5;
					break;
				case MiscID::rune_shield: //force
					lights[nrOfLights - 1].r = 0.0;
					lights[nrOfLights - 1].g = 0.5;
					lights[nrOfLights - 1].b = 0.0;
					break;
				//case STOMP:
				//	lights[nrOfLights - 1].r = 0.5;
				//	lights[nrOfLights - 1].g = 0.5;
				//	lights[nrOfLights - 1].b = 0.5;
				//	break;
				}
				lights[nrOfLights - 1].intensity = 2.0f;
				lights[nrOfLights - 1].distance = 15.0;
				lights[nrOfLights - 1].volume = 1;
				shrine->giveLight(&lights[nrOfLights - 1]);
			}
		}
		
		// --- Load World Collide ---
		worldCollide = new Rect**[35];
		for (int c = 0; c < 35; c++)
			worldCollide[c] = new Rect*[35];

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
					worldCollide[x][y] = 0;
			}
		}
	}
	else // --- INIT empty Map ---
	{	
		//default background
		chunkBackground = new GameObject();
		chunkBackground->init(0);
		chunkBackground->moveTo(xOffset * 35, yOffset * -35);
		
		if (xOffset == 0 && yOffset == 0)
		{
			GameObject* temp = new GameObject();
			temp->init(MiscID::shrine);
			temp->scaleUniformFactor(0);
			shrine = new Shrine(temp, MiscID(0));
		}
		else
			shrine = 0;
		musicId = -1;
		
		enemyMan = new EnemyManager;
		enemyMan->initEmpty();

		countWorldObjs = 0;

		nrOfLights = 0;

		worldCollide = new Rect**[35];
		for (int c = 0; c < 35; c++)
			worldCollide[c] = new Rect*[35];

		for (int y = 0; y < 35; y++)
			for (int x = 0; x < 35; x++)
			{
				worldCollide[x][y] = 0;
			}
	}
	in.close();
}

void MapChunk::initOld(int xIndex, int yIndex, std::string mapname)
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

	//load objects from file
	if (in.is_open())
	{
		mapnamepath = mapname;
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
				GameObject* temp = new GameObject();
				temp->init(1);
				temp->moveTo(xOffset * 35, yOffset * -35);
				temp->translate(pos.x, pos.y, pos.z);
				temp->scaleFactor(scale.x, scale.y, scale.z);
				gameObjects[WorldID::box].push_back(temp);
			}
			if (type == "Mushroom")
			{
				GameObject* temp = new GameObject();
				temp->init(3);
				temp->moveTo(xOffset * 35, yOffset * -35);
				temp->translate(pos.x, pos.y, pos.z);
				temp->scaleFactor(scale.x, scale.y, scale.z);
				gameObjects[WorldID::mushroom].push_back(temp);
			}
			if (type == "Shrine")
			{
				if (!shrine)
				{
					GameObject* shrineTemp = new GameObject();
					shrineTemp->init(MiscID::shrine); //2 = playerBase svart
					shrineTemp->moveTo(xOffset * 35, yOffset * -35);
					shrineTemp->translate(pos.x, pos.y, pos.z);
					shrineTemp->scaleFactor(scale.x, scale.y, scale.z);
					shrine = new Shrine(shrineTemp, MiscID(0));
				}
			}
		}
		//Load music id
		getline(in, line);
		iss = istringstream(line);
		iss >> sub;
		musicId = atoi(sub.c_str());

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
				lights[c].posX = atof(sub.c_str()) + xOffset * 35;
				iss >> sub;
				lights[c].posY = atof(sub.c_str()) - yOffset * 35;
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

		musicId = -1;

		enemyMan = new EnemyManager;
		enemyMan->initEmpty();

		countWorldObjs = 0;

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

void MapChunk::saveChunk()
{
	std::stringstream ss;
	ss << "../Spelprojekt/src/map/" << "map1" << "/"
		<< xOffset << "_" << yOffset << ".chunk";
	string fileName = ss.str();

	ofstream out;
	out.open(fileName, std::ofstream::out | std::ofstream::trunc);

	enemyMan->save(&out, xOffset, yOffset);

	//save background
	if (chunkBackground)
	{
		glm::mat4* mat = chunkBackground->getWorldMat();
		out << chunkBackground->returnID();
		out << " " << (*mat)[0].w - xOffset * 35 << " " << (*mat)[1].w + yOffset * 35 << " " << (*mat)[2].w << " :  BACKGROUND!" << endl;;
	}
	else
		out << -1 << " NO BACKGROUND" << endl;

	//save shrine
	if (shrine)
	{
		glm::mat4* mat = shrine->returnThis()->getWorldMat();
		out << shrine->getRune();
		out << " " << (*mat)[0].w - xOffset * 35 << " " << (*mat)[1].w + yOffset * 35 << " " << (*mat)[2].w << " :  SHRINE!" << endl;;
	}
	else
		out << -1 << " NO SHRINE" << endl;

	//save nr of world objects
	
	int count = 0;
	for (int n = 0; n < WorldID::world_count; n++)
		count += gameObjects[n].size();

	out << count << " :  Number of world objects" << endl;

	//save map objects
	for (int n = 0; n < WorldID::world_count; n++)
	{
		for (int k = 0; k < gameObjects[n].size(); k++)
		{
			out << gameObjects[n][k]->returnID() << " ";
			saveObject(gameObjects[n][k], &out);
		}
	}

	//save music id
	out << musicId << " : Music ID" << endl;

	//save nr of lights
	out << nrOfLights << " : Nr of Lights " << endl;

	//save lights   x y z  r g b intensity distance
	for (int n = 0; n < nrOfLights; n++)
	{
		out << lights[n].posX - xOffset*35 << " " << lights[n].posY + yOffset*35 << " " << lights[n].posZ;
		out << " " << lights[n].r << " " << lights[n].g << " " << lights[n].b;
		out << " " << lights[n].intensity << " " << lights[n].distance << " " << lights[n].volume << " : LIGHTS" << endl;
	}

	//save collision
	for (int y = 0; y < 35; y++)
	{
		for (int x = 0; x < 35; x++)
		{
			if (worldCollide[x][y])
				out << "X";
			else	
				out << "O";
		}
		out << endl;
	}
}

void MapChunk::saveObject(GameObject* object, ofstream* out)
{
	glm::mat4* mat;
	mat = object->getWorldMat();

	*out << ((*mat)[0].x) << " " << ((*mat)[0].y) << " " << ((*mat)[0].z) << " " << ((*mat)[0].w) - 35 * xOffset << " ";
	*out << ((*mat)[1].x) << " " << ((*mat)[1].y) << " " << ((*mat)[1].z) << " " << ((*mat)[1].w) + 35 * yOffset << " ";
	*out << ((*mat)[2].x) << " " << ((*mat)[2].y) << " " << ((*mat)[2].z) << " " << ((*mat)[2].w) << " ";
	*out << ((*mat)[3].x) << " " << ((*mat)[3].y) << " " << ((*mat)[3].z) << " " << ((*mat)[3].w) << endl;
}

void MapChunk::loadObject(ifstream* in)
{
	string line;
	string sub;

	int type;
	glm::mat4* mat = new glm::mat4;

	getline(*in, line);
	istringstream iss = istringstream(line);
	
	//Läs objekttyp
	iss >> sub;
	type = atoi(sub.c_str());
	
	//läs in world mat
	for (int n = 0; n < 4; n++)
	{
		iss >> sub;
		(*mat)[n].x = atof(sub.c_str());
		iss >> sub;
		(*mat)[n].y = atof(sub.c_str());
		iss >> sub;
		(*mat)[n].z = atof(sub.c_str());
		iss >> sub;
		(*mat)[n].w = atof(sub.c_str());
	}
	//create temp object
	GameObject* temp = new GameObject();
	temp->setWorldMat(mat);
	temp->translate(xOffset * 35, yOffset * -35);
	delete mat;

	temp->init(type);
	gameObjects[type].push_back(temp);
}

void MapChunk::loadObjectOld(ifstream* in)
{
	string line;
	string sub;

	string type;
	glm::mat4* mat = new glm::mat4;

	getline(*in, line);
	istringstream iss = istringstream(line);

	//Läs objekttyp
	iss >> sub;
	type = sub;

	//läs in world mat
	for (int n = 0; n < 4; n++)
	{
		iss >> sub;
		(*mat)[n].x = atof(sub.c_str());
		iss >> sub;
		(*mat)[n].y = atof(sub.c_str());
		iss >> sub;
		(*mat)[n].z = atof(sub.c_str());
		iss >> sub;
		(*mat)[n].w = atof(sub.c_str());
	}
	//create temp object
	GameObject* temp = new GameObject();
	temp->setWorldMat(mat);
	temp->translate(xOffset * 35, yOffset * -35);
	delete mat;

	if (type == "Box")
	{
		temp->init(WorldID::box);
		gameObjects[WorldID::box].push_back(temp);
	}
	else if (type == "Mushroom")
	{
		temp->init(WorldID::mushroom);
		gameObjects[WorldID::mushroom].push_back(temp);
	}
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

int MapChunk::update(float deltaTime, glm::vec3 playerPos, Map* map)
{
	int visitors = 0;
	if (enemyMan)
	{
		visitors = enemyMan->update(deltaTime, this, playerPos, map);
		Enemy** visitArr = enemyMan->getVisitors();
		for (int c = 0; c < visitors; c++)
		{
			map->findNewHome(visitArr[c]);
		}
	}
	if (shrine)
		shrine->update(deltaTime);
	return 0;
}

void MapChunk::respawnEnemies()
{
	if (shrine)
	{
		shrine->resetRune();
	}
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
					return hit = enemies[c]->readPos();
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
					return hit = enemies[c]->readPos();
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
					return hit = enemies[c]->readPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Cube");
	nrOfEnemies = enemyMan->size("Cube");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->readPos();
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
					return hit = boss->readPos();
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
					if (playerPos.x < enemies[c]->readPos().x)
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
					if (playerPos.x < enemies[c]->readPos().x)
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
					if (playerPos.x < enemies[c]->readPos().x)
						enemies[c]->hit(damage, false);
					else
						enemies[c]->hit(damage, true);
				}
			}
		}
	}
	enemies = enemyMan->getEnemies("Cube");
	nrOfEnemies = enemyMan->size("Cube");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			Rect* enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(wpnRect))
				{
					if (playerPos.x < enemies[c]->readPos().x)
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
				if (playerPos.x < boss->readPos().x)
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
		if (enemies[index])
		{
			if (enemies[index]->isAlive())
				return true;
		}
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
			if (!boss->isInitiated() || boss->isAlive() || boss->getType() == "ChuckTesta")
				return 1;
			else
				return 2;
		}
	}
	return 0;
}

Light* MapChunk::getFlameLight(int index)
{
	Enemy** enemies = enemyMan->getEnemies("Flame");
	if (enemies[index]->isAlive())
	{
		return ((Flame*)(enemies[index]))->getLight();
	}
	else if (((Flame*)(enemies[index]))->isFading())
	{
		((Flame*)(enemies[index]))->fade();
		return ((Flame*)(enemies[index]))->getLight();
	}
	return 0;
}

Light* MapChunk::getLights(int &lightNr)
{
	lightNr = nrOfLights;
	return lights;
}

int MapChunk::getMusicId()
{
	return musicId;
}

void MapChunk::initBoss()
{
	enemyMan->startBoss();
}

string MapChunk::getBossType()
{
	Enemy* boss = enemyMan->getBoss();
	if (boss)
		return boss->getType();
	return "ChuckTesta";
}

void MapChunk::recieveWorld(GameObject* item)
{
	gameObjects[item->returnID()].push_back(item);
}

GameObject* MapChunk::takeClosestWorldItem(glm::vec3 pos)
{
	GameObject* found = 0;
	float minDist = 999999;
	int type = 0;
	int index = 0;
	for (int n = 0; n < WorldID::world_count; n++)
	{
		for (int k = 0; k < gameObjects[n].size(); k++)
		{
			//compare
			float len = glm::length(gameObjects[n][k]->readPos() - pos);
			if (len < minDist)
			{
				found = gameObjects[n][k];
				minDist = len;
				type = n;
				index = k;
			}
		}
	}
	if (found) //remove item from chunk, fill it's pos with the last item in list
	{
		int last = gameObjects[type].size() - 1;
		gameObjects[type][index] = gameObjects[type][last];
		gameObjects[type].pop_back();
	}
	return found;
}

void MapChunk::addVisitor(Enemy* visitor, string type)
{
	enemyMan->addOutsider(visitor, type);
}

void MapChunk::addRekt(int x, int y)
{
	if (worldCollide[x][y])
		return;
	else
	{
		worldCollide[x][y] = new Rect();
		worldCollide[x][y]->initMapRect(xOffset, yOffset, x, y, 0);
	}
}

void MapChunk::removeRekt(int x, int y)
{
	if (worldCollide[x][y])
	{
		delete worldCollide[x][y];
		worldCollide[x][y] = 0;
	}
}

void MapChunk::recieveLight(Light* item)
{
	if (item)
	{
		Light* lightsTemp;
		lightsTemp = lights;
		lights = new Light[nrOfLights + 1];
		
		for (int n = 0; n < nrOfLights; n++)
		{
			lights[n] = lightsTemp[n];
		}
		delete[] lightsTemp;

		lights[nrOfLights] = *item;
		nrOfLights++;
	}
}