#include "mapChunk.h"
#include "map.h"
#include "Enemies/Flame.h"
#include "Enemies\ArcaneMissile.h"
#include "Enemies/Cube.h"
#include "Enemies\Ghost.h"

#include "Mirror.h"

MapChunk::~MapChunk()
{
	delete chunkBackground;
	delete enemyMan;

	if (shrine)
		delete shrine;
	
	if (health)
		delete health;

	if (nrOfLights > 0)
		delete[]lights;

	if (nrOfSounds > 0)
		delete[]sounds;

	for (int n = 0; n < WorldID::world_count; n++)
	{
		for (unsigned int k = 0; k < gameObjects[n].size(); k++)
		{
			delete gameObjects[n][k];
			
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

void MapChunk::init(int xIndex, int yIndex, std::string path, bool healthTaken)
{
	gameObjects = vector<vector<GameObject*>>();
	
	for (int n = 0; n < WorldID::world_count; n++)
	{
		vector<GameObject*> temp;
		gameObjects.push_back(temp);
	}

	//Build chunk filename
	std::stringstream ss;
	ss << path << "/" << xIndex << "_" << yIndex << ".chunk";
	string fileName = ss.str();

	ifstream in;
	in.open(fileName);
	
	
	xOffset = xIndex;
	yOffset = yIndex;

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
			pos.x = (float)atof(sub.c_str());
			ss >> sub;
			pos.y = (float)atof(sub.c_str());
			ss >> sub;
			pos.z = (float)atof(sub.c_str());
			chunkBackground = new GameObject();
			chunkBackground->init(id);
			chunkBackground->moveTo(pos);
			chunkBackground->translate((float)(xOffset * 35), (float)(yOffset * -35), 0);
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
			pos.x = (float)atof(sub.c_str());
			ss >> sub;
			pos.y = (float)atof(sub.c_str());
			ss >> sub;
			pos.z = (float)atof(sub.c_str());
			GameObject* temp = new GameObject();
			temp->init(MiscID::shrine);
			temp->moveTo(pos);
			temp->translate((float)(xOffset * 35), (float)(yOffset * -35), 0);
			shrine = new Shrine(temp, MiscID(id));
		}

		// --- Load health pickup ---
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		if (atoi(sub.c_str()) != -1)
		{
			if (!healthTaken)
			{
				health = new HealthPickup();
				ss >> sub;
				float xpos = (float)atof(sub.c_str());
				ss >> sub;
				float ypos = (float)atof(sub.c_str());
				health->init(glm::vec2(xpos + xOffset*35, ypos - yOffset*35));
			}
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

		// --- Load Sound count ---
		getline(in, line);
		ss = stringstream(line);
 		ss >> sub;
		nrOfSounds = atoi(sub.c_str());

		// --- Load Sounds ---
		if (nrOfSounds > 0)
		{
			int soundType; //0 = 2D, 1 = 3D
			bool is3D;
			int id;
			glm::vec3 soundPos;
			float dist;
			int loopType;
			float minInterval, maxInterval;

			sounds = new AudioObject[nrOfSounds];
			for (int c = 0; c < nrOfSounds; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				soundType = atoi(sub.c_str());//0 = 2D, 1 = 3D
				ss >> sub;
				id = atoi(sub.c_str());//id of the audio file
				ss >> sub;
				soundPos.x = (float)(atof(sub.c_str()) + xOffset * 35);
				ss >> sub;
				soundPos.y = (float)(atof(sub.c_str()) - yOffset * 35);
				ss >> sub;
				soundPos.z = (float)atof(sub.c_str());
				ss >> sub;
				dist = (float)atof(sub.c_str());// audible distance
				ss >> sub;
				loopType = atoi(sub.c_str());//0 = no loop, 1 = regular loop, 2 = interval based loop
				ss >> sub;
				minInterval = (float)atof(sub.c_str()); //min loop interval if loopType = 2
				ss >> sub;
				maxInterval = (float)atof(sub.c_str()); //max loop interval if loopType = 2

				if (soundType == 0)
					is3D = false;
				else
					is3D = true;

				sounds[c].init(is3D, id, soundPos, dist, loopType, minInterval, maxInterval);
			}
		}

		// --- Load Light count ---
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		nrOfLights = atoi(sub.c_str());

		//--- Load Lights --- 
		if (nrOfLights > 0)
		{
			lights = new Light[nrOfLights];
			for (int c = 0; c < nrOfLights; c++)
			{
				getline(in, line);
				ss = stringstream(line);
				ss >> sub;
				lights[c].posX = (float)(atof(sub.c_str()) + xOffset * 35);
				ss >> sub;
				lights[c].posY = (float)(atof(sub.c_str()) - yOffset * 35);
				ss >> sub;
				lights[c].posZ = (float)atof(sub.c_str());
				ss >> sub;
				lights[c].r = (float)atof(sub.c_str());
				ss >> sub;
				lights[c].g = (float)atof(sub.c_str());
				ss >> sub;
				lights[c].b = (float)atof(sub.c_str());
				ss >> sub;
				lights[c].intensity = (float)atof(sub.c_str());
				ss >> sub;
				lights[c].distance = (float)atof(sub.c_str());
				ss >> sub;
				lights[c].volume = atoi(sub.c_str());
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
					worldCollide[x][y]->initMapRect((float)xOffset, (float)yOffset, x, y, 0);
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
		chunkBackground->moveTo((float)(xOffset * 35), (float)(yOffset * -35));
		
		//if (xOffset == 0 && yOffset == 0)
		//{
		//	GameObject* temp = new GameObject();
		//	temp->init(MiscID::shrine);
		//	temp->scaleUniformFactor(0);
		//	shrine = new Shrine(temp, MiscID(0));
		//}
		//else
			shrine = 0;

		musicId = -1;
		
		nrOfSounds = 0;

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

void MapChunk::saveChunk(string path)
{
	std::stringstream ss;
	ss << path << "/" << xOffset << "_" << yOffset << ".chunk";
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

	//save shrined
	if (shrine)
	{
		glm::mat4* mat = shrine->returnThis()->getWorldMat();
		out << shrine->getRune();
		shrine->resetRune();
		out << " " << (*mat)[0].w - xOffset * 35 << " " << (*mat)[1].w + yOffset * 35 << " " << (*mat)[2].w << " :  SHRINE!" << endl;;
	}
	else
		out << -1 << " NO SHRINE" << endl;


	//save if we have HP pickup
	if (health)
	{
		glm::vec3 hpos = health->readPos();
		out << "0 " << hpos.x - xOffset * 35 << " " << hpos.y + yOffset * 35 << "\n";
	}
	else
		out << "-1 NO HEALTH PICKUP\n";

	//save nr of world objects
	
	int count = 0;
	for (int n = 0; n < WorldID::world_count; n++)
		count += gameObjects[n].size();

	out << count << " :  Number of world objects" << endl;

	//save map objects
	for (int n = 0; n < WorldID::world_count; n++)
	{
		for (unsigned int k = 0; k < gameObjects[n].size(); k++)
		{
			out << gameObjects[n][k]->returnID() << " ";
			saveObject(gameObjects[n][k], &out);
		}
	}

	//save music id
	out << musicId << " : Music ID" << endl;

	//Save all chunk sounds
	out << nrOfSounds << " : Nr of Sounds (type, id, pos.x, pos.y, pos.z, distance, loopType, minInverval, maxInterval)" << endl;
	int type, id, loopType;
	float x, y, z, dist, minInterval, maxInterval;
	for (int c = 0; c < nrOfSounds; c++)
	{
		sounds[c].getAllValues(type, id, x, y, z, dist, loopType, minInterval, maxInterval);
		out << type << " " << id << " " << x - xOffset*35 << " " << y + yOffset*35 << " " << z << " " << dist << " " << loopType << " " << minInterval << " " << maxInterval << endl;
	}
	//1 : Nr of Sounds(type, id, pos.x, pos.y, pos.z, distance, loopType, minInverval, maxInterval)
	//	1 19 0.0 0.0 0.0 20.0 2 10 50

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
	*out << ((*mat)[3].x) << " " << ((*mat)[3].y) << " " << ((*mat)[3].z) << " " << ((*mat)[3].w) << " ";

	if (object->returnID() == WorldID::mirror)
	{
		glm::vec2 tp = ((Mirror*)object)->getTeleportLocation();
		glm::vec2 ch = ((Mirror*)object)->getChunkTeleport();
		*out << ch.x << " " << ch.y << " " << tp.x - ch.x*35 << " " << tp.y + ch.y*35;
	}

	*out << endl;
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
		(*mat)[n].x = (float)atof(sub.c_str());
		iss >> sub;
		(*mat)[n].y = (float)atof(sub.c_str());
		iss >> sub;
		(*mat)[n].z = (float)atof(sub.c_str());
		iss >> sub;
		(*mat)[n].w = (float)atof(sub.c_str());
	}
	//create temp object
	if (type == WorldID::mirror)
	{
		glm::vec2 chunk;
		iss >> sub;
		chunk.x = (float)atof(sub.c_str());
		iss >> sub;
		chunk.y = (float)atof(sub.c_str());
		glm::vec2 teleport;
		iss >> sub;
		teleport.x = (float)atof(sub.c_str());
		iss >> sub;
		teleport.y = (float)atof(sub.c_str());
		GameObject* temp = new Mirror();
		temp->setWorldMat(mat);
		((Mirror*)temp)->setTeleportLocation(teleport, chunk);
		temp->translate((float)(xOffset * 35), (float)(yOffset * -35));
		((Mirror*)temp)->setRect();
		delete mat;

		temp->init(type);
		gameObjects[type].push_back(temp);
	}
	else
	{
		GameObject* temp = new GameObject();
		temp->setWorldMat(mat);
		temp->translate((float)(xOffset * 35), (float)(yOffset * -35));
		delete mat;

		temp->init(type);
		gameObjects[type].push_back(temp);
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

	if (health)
		health->update(deltaTime);

	if (nrOfSounds > 0)
	{
		for (int i = 0; i < nrOfSounds; i++)
			sounds[i].update(deltaTime);
	}

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
	Rect* enemyRect = 0;

	Enemy** enemies = enemyMan->getEnemies("Flame");
	int nrOfEnemies = enemyMan->size("Flame");
	glm::vec3 hit = glm::vec3(0,0,-1);
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
			enemyRect = enemies[c]->getRekt();
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
			enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->readPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Deathbox");
	nrOfEnemies = enemyMan->size("Deathbox");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		enemyRect = enemies[c]->getRekt();
		if (enemyRect)
		{
			if (enemyRect->intersects(playerRect))
			{
				hit = enemies[c]->readPos();
				hit.z = 10.0f;
				return hit;
			}
		}
	}
	enemies = enemyMan->getEnemies("Cube");
	nrOfEnemies = enemyMan->size("Cube");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->readPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Spider");
	nrOfEnemies = enemyMan->size("Spider");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->readPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Ghost");
	nrOfEnemies = enemyMan->size("Ghost");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive() && !enemies[c]->isBlinking())
		{
			enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->readPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Spellbook");
	nrOfEnemies = enemyMan->size("Spellbook");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = enemies[c]->readPos();
			}
		}
	}
	enemies = enemyMan->getEnemies("Missile");
	nrOfEnemies = enemyMan->size("Missile");
	for (int c = 0; c < nrOfEnemies && hit.z == -1; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
			enemyRect = boss->getRekt();
			if (enemyRect)
			{
				if (enemyRect->intersects(playerRect))
					return hit = boss->readPos();
			}
			if (boss->getType() == "Grim")
			{
				enemies = enemyMan->getEnemies("GrimHand");
				for (int c = 0; c < 2; c++)
				{
					if (enemies[c])
					{
						enemyRect = enemies[c]->getRekt();
						if (enemyRect)
						{
							if (enemyRect->intersects(playerRect))
								return hit = boss->readPos();
						}
					}
				}

				enemies = enemyMan->getEnemies("GrimLaser");
				if (enemies[0])
				{
					enemyRect = enemies[0]->getRekt();
					if (enemyRect)
					{
						if (enemyRect->intersects(playerRect))
							return hit = boss->readPos();
					}
				}
				enemies = enemyMan->getEnemies("GrimScythe");
				if (enemies[0])
				{
					enemyRect = enemies[0]->getRekt();
					if (enemyRect)
					{
						if (enemyRect->intersects(playerRect))
							return hit = boss->readPos();
					}
				}
			}
		}
	}
	return hit;
}

void MapChunk::attackEnemies(Rect* wpnRect, glm::vec3 playerPos, int damage)
{
	Rect* enemyRect = 0;
	Enemy** enemies = enemyMan->getEnemies("Bat");
	int nrOfEnemies = enemyMan->size("Bat");

	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
			enemyRect = enemies[c]->getRekt();
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
			enemyRect = enemies[c]->getRekt();
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
			enemyRect = enemies[c]->getRekt();
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
	enemies = enemyMan->getEnemies("Spider");
	nrOfEnemies = enemyMan->size("Spider");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
	enemies = enemyMan->getEnemies("Ghost");
	nrOfEnemies = enemyMan->size("Ghost");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
	enemies = enemyMan->getEnemies("Spellbook");
	nrOfEnemies = enemyMan->size("Spellbook");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
	enemies = enemyMan->getEnemies("Webshot");
	nrOfEnemies = enemyMan->size("Webshot");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
	enemies = enemyMan->getEnemies("Missile");
	nrOfEnemies = enemyMan->size("Missile");
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (enemies[c]->isAlive())
		{
			enemyRect = enemies[c]->getRekt();
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
		enemyRect = boss->getHurtRekt();
		if (enemyRect)
		{
			if (enemyRect->intersects(wpnRect))
			{
				if (playerPos.x < boss->readPos().x)
					boss->hit(damage, false);
				else
					boss->hit(damage, true);
			}
		}
		if (boss->getType() == "Grim")
		{
			enemies = enemyMan->getEnemies("GrimHand");
			for (int c = 0; c < 2; c++)
			{
				if (enemies[c])
				{
					enemyRect = enemies[c]->getHurtRekt();
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
				Debug::DebugOutput("Hittade en respawnpunkt!\n");
				Audio::getAudio().playSound(SoundID::player_saved, false);
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
		if (enemies[index])
			return enemies[index]->isBlinking();
	}
	return false;
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

Light* MapChunk::getFlameLight(int index, int &nrLigts)
{
	Enemy** enemies = enemyMan->getEnemies("Flame");
	if (enemies[index]->isAlive())
	{
		return ((Flame*)(enemies[index]))->getLight(nrLigts);
	}
	else if (((Flame*)(enemies[index]))->isFading())
	{
		((Flame*)(enemies[index]))->fade();
		return ((Flame*)(enemies[index]))->getLight(nrLigts);
	}
	return 0;
}

Light* MapChunk::getMissileLight(int index, int &nrLights)
{
	Enemy** enemies = enemyMan->getEnemies("Missile");
	if (enemies[index]->isAlive())
	{
		return ((ArcaneMissile*)(enemies[index]))->getLight(nrLights);
	}
	else
	{
		((ArcaneMissile*)(enemies[index]))->fade();
		return ((ArcaneMissile*)(enemies[index]))->getLight(nrLights);
	}

	return 0;
}

Light* MapChunk::getGhostLight(int index, int &nrLights)
{
	Enemy** enemies = enemyMan->getEnemies("Ghost");
	bool isAlive = enemies[index]->isAlive();
	bool isBlink = enemies[index]->isBlinking();
	if (!isBlink && isAlive)
	{
		return ((Ghost*)(enemies[index]))->getLight(nrLights);
	}
	else
	{
		((Ghost*)(enemies[index]))->fade();
		return ((Ghost*)(enemies[index]))->getLight(nrLights);
	}
	return 0;
}

Light* MapChunk::getCubeGlows(int index)
{
	Enemy** enemies = enemyMan->getEnemies("Cube");
	if (enemies[index]->isAlive())
		return ((Cube*)(enemies[index]))->getGlow();
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
	if (item->returnID() == WorldID::mirror)
	{
		GameObject* temp = new Mirror();
		temp->coppyMat(item);
		temp->init(item->returnID());
		gameObjects[item->returnID()].push_back(temp);
	}
	else
	{
		gameObjects[item->returnID()].push_back(item);
	}
	
}

GameObject* MapChunk::takeClosestWorldItem(glm::vec3 pos)
{
	GameObject* found = 0;
	float minDist = 999999;
	int type = 0;
	int index = 0;
	for (int n = 0; n < WorldID::world_count; n++)
	{
		for (unsigned int k = 0; k < gameObjects[n].size(); k++)
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
		worldCollide[x][y]->initMapRect((float)xOffset, (float)yOffset, x, y, 0);
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
		if (nrOfLights <= 0)
		{
			lights = new Light[1];
			lights[0] = *item;
			nrOfLights++;
		}
		else
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
}

Light* MapChunk::takeClosestLight(glm::vec3 pos)
{
	if (nrOfLights > 0)
	{
		Light* found = new Light();
		float minDist = 999999;
		int index = 0;
		
		for (int n = 0; n < nrOfLights; n++)
		{
			glm::vec3 dist = glm::vec3(lights[n].posX, lights[n].posY, lights[n].posZ);
			float distance = glm::length(dist - pos);
			if (distance < minDist)
			{
				minDist = distance;
				index = n;
			}
		}
		*found = lights[index];

		lights[index] = lights[nrOfLights - 1];

		Light* temp = lights;
		lights = new Light[nrOfLights - 1];
		nrOfLights--;
		for (int n = 0; n < nrOfLights; n++)
		{
			lights[n] = temp[n];
		}
		delete[] temp;
		return found;
	}
	else 
		return 0;
}

bool MapChunk::takePickup(Rect* playerRect)
{
	if (health)
	{
		if (health->isTaken())
			return false;
		if (playerRect->intersects(health->getRekt()))
		{
			health->take();
			return true;
		}

	}
	return false;
}

HealthPickup* MapChunk::getPickup()
{
	return health;
}

bool MapChunk::playerVsWeb(Rect* pRect)
{
	Enemy** webs = enemyMan->getEnemies("Web");
	int nrOfWebs = enemyMan->size("Web");
	for (int c = 0; c < nrOfWebs; c++)
	{
		if (webs[c]->getRekt()->intersects(pRect))
			return true;
	}
	return false;
}

glm::vec3 MapChunk::teleportTo(Rect* pRect)
{
	glm::vec3 hit = glm::vec3(0, 0, -1);
	int size = gameObjects[WorldID::mirror].size();
	bool found = false;
	for (int c = 0; c < size && !found; c++)
	{
		Rect* temp = ((Mirror*)gameObjects[WorldID::mirror][c])->getRekt();
		if (pRect->intersects(temp))
		{
			glm::vec2 tpLoc = ((Mirror*)gameObjects[WorldID::mirror][c])->getTeleportLocation();
			hit = glm::vec3(tpLoc, 0);
			found = true;
		}
	}
	return hit;
}