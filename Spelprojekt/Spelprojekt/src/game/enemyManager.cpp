#include "enemyManager.h"
#include "mapChunk.h"
#include "map.h"
#include "Enemies/Spikes.h"
#include "Enemies/Bat.h"
#include "Enemies/Flame.h"
#include "Enemies/Bossbat.h"
#include "Enemies/Bossspider.h"
#include "Enemies/Bossghost.h"
#include "Enemies/Bossdummy.h"
#include "Enemies/Cube.h"
#include "Enemies/Spider.h"
#include "Enemies/Ghost.h"
#include "Web.h"
#include "Enemies/Webshot.h"
#include "Enemies/Spellbook.h"
#include "Enemies/ArcaneMissile.h"
#include "Enemies/Deathbox.h"

#include "Enemies/Grim.h"
#include "Enemies/GrimHand.h"
#include "Enemies/GrimScythe.h"
#include "Enemies/GrimLaser.h"
#include <sstream>

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{
	for (int c = 0; c < batCount; c++)
		delete bats[c];
	delete[]bats;
	for (int c = 0; c < flameCount; c++)
		delete flames[c];
	delete[]flames;
	for (int c = 0; c < spikeCount; c++)
		delete spikes[c];
	delete[]spikes;
	for (int c = 0; c < cubeCount; c++)
		delete cubes[c];
	delete[]cubes;
	for (int c = 0; c < spiderCount; c++)
		delete spiders[c];
	delete[]spiders;
	for (int c = 0; c < ghostCount; c++)
		delete ghosts[c];
	delete[]ghosts;
	for (int c = 0; c < webCount; c++)
		delete webs[c];
	delete[]webs;
	for (int c = 0; c < shotCount; c++)
		delete webShots[c];
	delete[]webShots;
	for (int c = 0; c < spellbookCount; c++)
		delete spellbooks[c];
	delete[]spellbooks;
	for (int c = 0; c < missileCount; c++)
		delete missiles[c];
	delete[]missiles;
	for (int c = 0; c < deathCount; c++)
		delete deathboxes[c];
	delete[]deathboxes;

	if (boss)
		delete boss;
	for (int c = 0; c < 2; c++)
	{
		if (grimHands[c])
		{
			delete grimHands[c];
			grimHands[c] = 0;
		}
	}
	if (grimScythe)
		delete grimScythe;
	if (grimLaser)
		delete grimLaser;


	if (visitorHolder)
		delete[]visitorHolder;
}

void EnemyManager::init(ifstream &file, int xOffset, int yOffset)
{
	batCount = 0; batMax = 5;
	bats = new Enemy*[batMax];
	spikeCount = 0; spikeMax = 5;
	spikes = new Enemy*[spikeMax];
	flameCount = 0; flameMax = 5;
	flames = new Enemy*[flameMax];
	cubeCount = 0; cubeMax = 5;
	cubes = new Enemy*[cubeMax];
	spiderCount = 0; spiderMax = 5;
	spiders = new Enemy*[spiderMax];
	ghostCount = 0; ghostMax = 5;
	ghosts = new Enemy*[ghostMax];
	webCount = 0; webMax = 5;
	webs = new Enemy*[webMax];
	shotCount = 0; shotMax = 5;
	webShots = new Enemy*[shotMax];
	spellbookCount = 0; spellbookMax = 5;
	spellbooks = new Enemy*[spellbookMax];
	missileCount = 0; missileMax = 5;
	missiles = new Enemy*[missileMax];
	deathCount = 0; deathMax = 5;
	deathboxes = new Enemy*[deathMax];

	boss = 0;
	grimHands[0] = grimHands[1] = grimLaser = grimScythe = 0;
	string line;
	string type; //To store type
	glm::vec2 pos; //To store pos

	//Find all enemies, create them, and load them
	
	getline(file, line);
	istringstream iss(line);
	string sub;
	iss >> sub;
	if (atoi(sub.c_str()) == 1) //THERE IS A BOSS HOLY HELL
	{
		iss >> sub;
		type = sub; //Läs bosstyp
		iss >> sub;
		pos.x = (float)atof(sub.c_str());
		pos.x = pos.x + xOffset * 35;
		iss >> sub;
		pos.y = (float)atof(sub.c_str());
		pos.y = pos.y - yOffset * 35;
		addBoss(type, pos);
	}
	getline(file, line);
	iss = istringstream(line);
	iss >> sub;
	int nrOfEnemies = atoi(sub.c_str());
	for (int c = 0; c < nrOfEnemies; c++)
	{
		if (!(getline(file, line))) break;
		iss = istringstream(line);
		iss >> sub;
		type = sub; //Läs fiendetyp
		iss >> sub;
		pos.x = (float)atof(sub.c_str());
		pos.x = pos.x + xOffset * 35;
		iss >> sub;
		pos.y = (float)atof(sub.c_str());
		pos.y = pos.y - yOffset * 35;
		addEnemy(type, pos);
	}
}

void EnemyManager::save(ofstream* out, int xOffset, int yOffset)
{
	//reset all enemies to their original pos
	resetEnemies();

	//save boss 
	if (boss)
	{
		glm::vec2 bossPos = boss->plsGiveBossPos();
		*out << 1 << " " << boss->getType() << " " << bossPos.x - xOffset * 35 << " " << bossPos.y + yOffset * 35 << endl;
	}
	else
		*out << 0 << " : ChuckTesta" << endl;
	*out << batCount + flameCount + spikeCount + cubeCount + spiderCount + ghostCount + webCount + spellbookCount + deathCount << " : Enemy Count" << endl;

	for (int n = 0; n < batCount; n++)
		*out << "Bat " << bats[n]->readPos().x - xOffset * 35 << " " << bats[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < flameCount; n++)
		*out << "Flame " << flames[n]->readPos().x - xOffset * 35 << " " << flames[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < spikeCount; n++)
		*out << "Spikes " << spikes[n]->readPos().x - xOffset * 35 << " " << spikes[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < cubeCount; n++)
		*out << "Cube " << cubes[n]->readPos().x - xOffset * 35 << " " << cubes[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < spiderCount; n++)
		*out << "Spider " << spiders[n]->readPos().x - xOffset * 35 << " " << spiders[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < ghostCount; n++)
		*out << "Ghost " << ghosts[n]->readPos().x - xOffset * 35 << " " << ghosts[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < webCount; n++)
		*out << "Web " << webs[n]->readPos().x - xOffset * 35 << " " << webs[n]->readPos().y + yOffset * 35 << endl;
	
	for (int n = 0; n < spellbookCount; n++)
		*out << "Spellbook " << spellbooks[n]->readPos().x - xOffset * 35 << " " << spellbooks[n]->readPos().y + yOffset * 35 << endl;

	for (int n = 0; n < deathCount; n++)
		*out << "Deathbox " << deathboxes[n]->readPos().x - xOffset * 35 << " " << deathboxes[n]->readPos().y + yOffset * 35 << endl;
}

void EnemyManager::initEmpty()
{
	flameCount = -1;
	flames = 0;
	spikeCount = -1;
	spikes = 0;
	batCount = -1;
	bats = 0;
	spiderCount = -1;
	spiders = 0;
	ghostCount = -1;
	ghosts = 0;
	webCount = -1;
	webs = 0;
	shotCount = -1;
	webShots = 0;
	ghosts;
	spellbookCount = -1;
	spellbooks = 0;
	missileCount = -1;
	missiles = 0;
	deathCount = -1;
	deathboxes = 0;

	boss = 0;
	grimHands[0] = grimHands[1] = grimLaser = grimScythe = 0;
}

int EnemyManager::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos, Map* map)
{
	int idX = -1, idY = -1;
	if (visitorHolder)
	{
		for (int c = 0; c < visitorsToSendOut; c++)
			delete visitorHolder[c];
		delete[]visitorHolder;
	}
	visitorsToSendOut = 0;
	int maxVisitors = 5;
	visitorHolder = new Enemy*[maxVisitors];

	int msg = 0;
	glm::vec3 chunkMid = glm::vec3(chunk->xOffset * 35, chunk->yOffset * -35, 0);
	glm::vec3 pos;
	for (int c = 0; c < batCount; c++)
	{
		if (bats[c]->isAlive())
		{
			msg = bats[c]->update(deltaTime, map, playerPos);
			pos = bats[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Bat* visitBat = new Bat((Bat*)bats[c]);
				visitBat->setVisitor();
				bats[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitBat;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < flameCount; c++)
	{
		if (flames[c]->isAlive())
		{
			msg = flames[c]->update(deltaTime, map, playerPos);
			pos = flames[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Flame* visitFlame = new Flame((Flame*)flames[c]);
				visitFlame->setVisitor();
				flames[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitFlame;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < cubeCount; c++)
	{
		if (cubes[c]->isAlive())
		{
			msg = cubes[c]->update(deltaTime, map, playerPos);
			pos = cubes[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Cube* visitCube = new Cube((Cube*)cubes[c]);
				visitCube->setVisitor();
				cubes[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitCube;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < spiderCount; c++)
	{
		if (spiders[c]->isAlive())
		{
			msg = spiders[c]->update(deltaTime, map, playerPos);
			pos = spiders[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Spider* visitSpider = new Spider((Spider*)spiders[c]);
				visitSpider->setVisitor();
				spiders[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitSpider;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < ghostCount; c++)
	{
		if (ghosts[c]->isAlive())
		{
			msg = ghosts[c]->update(deltaTime, map, playerPos);
			pos = ghosts[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Ghost* visitGhost = new Ghost((Ghost*)ghosts[c]);
				visitGhost->setVisitor();
				ghosts[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitGhost;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	if (boss)
	{
		Debug::DebugOutput("");
	}
	for (int c = 0; c < shotCount; c++)
	{
		msg = webShots[c]->update(deltaTime, map, playerPos);
		if (msg)
		{
			delete webShots[c];
			webShots[c] = webShots[--shotCount];
			c--;
		}
	}
	for (int c = 0; c < spellbookCount; c++)
	{
		if (spellbooks[c]->isAlive())
		{
			msg = spellbooks[c]->update(deltaTime, map, playerPos);
			pos = spellbooks[c]->readPos();
			if (pos.x < chunkMid.x - 17.5f || pos.x > chunkMid.x + 17.5f ||
				pos.y < chunkMid.y - 17.5f || pos.y > chunkMid.y + 17.5f)
			{
				Spellbook* visitBook = new Spellbook((Spellbook*)spellbooks[c]);
				visitBook->setVisitor();
				spellbooks[c]->diePls();
				visitorHolder[visitorsToSendOut] = visitBook;
				visitorsToSendOut++;
				if (visitorsToSendOut == maxVisitors)
					expandEnemyArray(visitorHolder, maxVisitors);
			}
		}
	}
	for (int c = 0; c < missileCount; c++)
	{
		msg = missiles[c]->update(deltaTime, map, playerPos);
		if (msg)
		{
			//((ArcaneMissile*)missiles[c])->fade();
			if (!((ArcaneMissile*)missiles[c])->isFading())
			{
				delete missiles[c];
				missiles[c] = missiles[--missileCount];
				c--;
			}
		}
	}

	if (boss)
	{
		if (boss->isAlive())
		{
			msg = boss->update(deltaTime, map, playerPos);
			if (boss->getType() == "Bossghost" && msg == 1)
			{
				for (int c = 0; c < ghostCount; c++)
				{
					ghosts[c]->diePls();
				}
			}
			if (boss->getType() == "Grim" && msg != 5)
			{
				for (int c = 0; c < 2; c++)
				{
					if (grimHands[c])
					{
						((GrimHand*)grimHands[c])->setGrimMode(msg);
						grimHands[c]->update(deltaTime, map, playerPos);
					}
				}
			}
			if (msg > 4)
			{
				for (int c = 0; c < 2; c++)
				{
					if (grimHands[c])
					{
						delete grimHands[c];
						grimHands[c] = 0;
					}
				}

				if (grimScythe)
				{
					if (msg == 10)
						((GrimScythe*)grimScythe)->returnToMaster();
					grimScythe->update(deltaTime, map, playerPos);
				}
				else
				{
					addEnemy("GrimScythe", glm::vec2(boss->readPos()));
					grimScythe->init();
				}

				if (msg == 6)
				{
					if (!grimLaser)
						addEnemy("GrimLaserH", glm::vec2(boss->readPos()));
				}
				else if (msg == 7)
				{
					if (!grimLaser)
						addEnemy("GrimLaserV", glm::vec2(boss->readPos()));
				}

				if (grimLaser)
				{
					msg = grimLaser->update(deltaTime, map, playerPos);
					if (msg)
					{
						delete grimLaser;
						grimLaser = 0;
					}
				}
			}
		}
	}
	return visitorsToSendOut;
}

int EnemyManager::size(string type)
{
	if (type == "Spikes")
		return spikeCount;
	if (type == "Bat")
		return batCount;
	if (type == "Flame")
		return flameCount;
	if (type == "Cube")
		return cubeCount;
	if (type == "Spider")
		return spiderCount;
	if (type == "Ghost")
		return ghostCount;
	if (type == "Web")
		return webCount;
	if (type == "Webshot")
		return shotCount;
	if (type == "Spellbook")
		return spellbookCount;
	if (type == "Missile")
		return missileCount;
	if (type == "Deathbox")
		return deathCount;

	if (type == "GrimHand")
	{
		if (grimHands[0])
			return 2;
		return 0;
	}

	return 0;
}

void EnemyManager::addEnemy(string type, glm::vec2 pos)
{
	if (type == "Spikes")
	{
		spikes[spikeCount] = new Spikes(pos);
		spikeCount++;
		if (spikeCount == spikeMax)
			expandEnemyArray(spikes, spikeMax);
	}
	else if (type == "Deathbox")
	{
		deathboxes[deathCount] = new Deathbox(pos);
		deathCount++;
		if (deathCount == deathMax)
			expandEnemyArray(deathboxes, deathMax);
	}
	else if (type == "Bat")
	{
		bats[batCount] = new Bat(pos);
		batCount++;
		if (batCount == batMax)
			expandEnemyArray(bats, batMax);
	}
	else if (type == "Flame")
	{
		flames[flameCount] = new Flame(pos);
		flameCount++;
		if (flameCount == flameMax)
			expandEnemyArray(flames, flameMax);
	}
	else if (type == "Cube")
	{
		cubes[cubeCount] = new Cube(pos);
		cubeCount++;
		if (cubeCount == cubeMax)
			expandEnemyArray(cubes, cubeMax);
	}
	else if (type == "Spider")
	{
		spiders[spiderCount] = new Spider(pos);
		spiderCount++;
		if (spiderCount == spiderMax)
			expandEnemyArray(spiders, spiderMax);
	}
	else if (type == "Ghost")
	{
		ghosts[ghostCount] = new Ghost(pos);
		ghostCount++;
		if (ghostCount == ghostMax)
			expandEnemyArray(ghosts, ghostMax);
	}
	else if (type == "Web")
	{
		webs[webCount] = new Web(pos);
		webCount++;
		if (webCount == webMax)
			expandEnemyArray(webs, webMax);
	}
	else if (type == "Spellbook")
	{
		spellbooks[spellbookCount] = new Spellbook(pos);
		spellbookCount++;
		if (spellbookCount == spellbookMax)
			expandEnemyArray(spellbooks, spellbookMax);
	}
	else if (type == "GrimHand")
	{
		if (grimHands[0])
		{
			grimHands[1] = new GrimHand(pos);
			grimHands[1]->setContentIndex(EnemyID::grimhand_left);
		}
		else
		{
			grimHands[0] = new GrimHand(pos);
			grimHands[0]->setContentIndex(EnemyID::grimhand_right);
		}
	}
	else if (type == "GrimScythe")
	{
		if (grimScythe)
			delete grimScythe;
		grimScythe = new GrimScythe(pos);
	}
	else if (type == "GrimLaserV")
	{
		if (grimLaser)
			delete grimLaser;
		grimLaser = new GrimLaser(pos, true);
	}
	else if (type == "GrimLaserH")
	{
		if (grimLaser)
			delete grimLaser;
		grimLaser = new GrimLaser(pos, false);
	}
}

int EnemyManager::bindEnemy(int index, GLuint* shader, GLuint* uniform, string type)
{
	if (index == -1)
	{
		if (boss)
			return boss->bindWorldMat(shader, uniform);
	}
	else if (type == "Bat")
		return bats[index]->bindWorldMat(shader, uniform);
	else if (type == "Flame")
		return flames[index]->bindWorldMat(shader, uniform);
	else if (type == "Spikes")
		return spikes[index]->bindWorldMat(shader, uniform);
	else if (type == "Cube")
		return cubes[index]->bindWorldMat(shader, uniform);
	else if (type == "Spider")
		return spiders[index]->bindWorldMat(shader, uniform);
	else if (type == "Ghost")
		return ghosts[index]->bindWorldMat(shader, uniform);
	else if (type == "Web")
		return webs[index]->bindWorldMat(shader, uniform);
	else if (type == "Webshot")
		return webShots[index]->bindWorldMat(shader, uniform);
	else if (type == "Spellbook")
		return spellbooks[index]->bindWorldMat(shader, uniform);
	else if (type == "Missile")
		return missiles[index]->bindWorldMat(shader, uniform);
	else if (type == "Deathbox")
		return deathboxes[index]->bindWorldMat(shader, uniform);

	else if (type == "GrimHand")
		return grimHands[index]->bindWorldMat(shader, uniform);
	else if (type == "GrimLaser")
		return grimLaser->bindWorldMat(shader, uniform);
	else if (type == "GrimScythe")
		return grimScythe->bindWorldMat(shader, uniform);

	return -1;
}

Enemy** EnemyManager::getEnemies(string type)
{
	if (type == "Spikes")
		return spikes;
	if (type == "Bat")
		return bats;
	if (type == "Flame")
		return flames;
	if (type == "Cube")
		return cubes;
	if (type == "Spider")
		return spiders;
	if (type == "Ghost")
		return ghosts;
	if (type == "Web")
		return webs;
	if (type == "Webshot")
		return webShots;
	if (type == "Spellbook")
		return spellbooks;
	if (type == "Missile")
		return missiles;
	if (type == "Deathbox")
		return deathboxes;

	if (type == "GrimHand")
		return grimHands;
	if (type == "GrimScythe")
		return &grimScythe;
	if (type == "GrimLaser")
		return &grimLaser;
	return 0;
}

void EnemyManager::resetEnemies()
{
	for (int n = batCount-1; n >= 0; n--)
	{
		if (bats[n]->isVisitor())
		{
			delete bats[n];
			bats[n] = 0;
			batCount--;
		}
		else
			bats[n]->init();
	}
	for (int n = flameCount-1; n >= 0; n--)
	{
		if (flames[n]->isVisitor())
		{
			delete flames[n];
			flames[n] = 0;
			flameCount--;
		}
		else
			flames[n]->init();
	}
	for (int n = cubeCount - 1; n >= 0; n--)
	{
		if (cubes[n]->isVisitor())
		{
			delete cubes[n];
			cubes[n] = 0;
			cubeCount--;
		}
		else
			cubes[n]->init();
	}
	for (int n = spiderCount - 1; n >= 0; n--)
	{
		if (spiders[n]->isVisitor())
		{
			delete spiders[n];
			spiders[n] = 0;
			spiderCount--;
		}
		else
			spiders[n]->init();
	}
	for (int n = ghostCount - 1; n >= 0; n--)
	{
		if (ghosts[n]->isVisitor())
		{
			delete ghosts[n];
			ghosts[n] = 0;
			ghostCount--;
		}
		else
			ghosts[n]->init();
	}
	for (int c = webCount - 1; c >= 0; c--)
	{
		if (webs[c]->isVisitor())
		{
			delete webs[c];
			webs[c] = 0;
			webCount--;
		}
		else
			webs[c]->init();
	}
	for (int c = shotCount - 1; c >= 0; c--)
	{
		delete webShots[c];
		webShots[c] = 0;
		shotCount--;
	}
	for (int n = spellbookCount - 1; n >= 0; n--)
	{
		if (spellbooks[n]->isVisitor())
		{
			delete spellbooks[n];
			spellbooks[n] = 0;
			spellbookCount--;
		}
		else
			spellbooks[n]->init();
	}
	for (int c = missileCount - 1; c >= 0; c--)
	{
		delete missiles[c];
		missiles[c] = 0;
		missileCount--;
	}
	
	if (boss)
	{
		if (boss->isAlive())
		{
			boss->init();
		}
	}
}

Enemy* EnemyManager::getBoss()
{
	return boss;
}

void EnemyManager::addBoss(string type, glm::vec2 pos)
{
	if (type == "ChuckTesta")
	{
		boss = new Dummy(pos);
		boss->scaleFactor(0.01f, 0.01f, 0.01f);
	}
	else if (type == "Bossbat")
	{
		boss = new Bossbat(pos);
	}
	else if (type == "Bossspider")
		boss = new Bossspider(pos);
	else if (type == "Bossghost")
		boss = new Bossghost(pos);
	else if (type == "Grim")
		boss = new Grim(pos);
}

void EnemyManager::startBoss()
{
	boss->init();
	if (boss->getType() == "Grim")
	{
		for (int c = 0; c < 2; c++)
		{
			if (grimHands[c])
			{
				delete grimHands[c];
				grimHands[c] = 0;
			}
		}
		if (grimLaser)
		{
			delete grimLaser;
			grimLaser = 0;
		}
		if (grimScythe)
		{
			delete grimScythe;
			grimScythe = 0;
		}
		glm::vec2 handPos = ((Grim*)boss)->getHandPos();
		addEnemy("GrimHand", glm::vec2(handPos.x + 35, handPos.y));
		addEnemy("GrimHand", glm::vec2(-handPos.x + 35, handPos.y));
		grimHands[0]->init();
		grimHands[1]->init();
	}
}

void EnemyManager::expandEnemyArray(Enemy**& arr, int &oldMax)
{
	Enemy** temparr = new Enemy*[oldMax + 5];
	for (int c = 0; c < oldMax; c++)
	{
		temparr[c] = arr[c];
	}
	delete[]arr;
	arr = temparr;
	oldMax += 5;
}

void EnemyManager::addOutsider(Enemy* visitor, string type)
{
	if (type == "Bat")
	{
		bats[batCount] = new Bat((Bat*)visitor);
		batCount++;
		if (batCount == batMax)
			expandEnemyArray(bats, batMax);
	}
	if (type == "Flame")
	{
		flames[flameCount] = new Flame((Flame*)visitor);
		flameCount++;
		if (flameCount == flameMax)
			expandEnemyArray(flames, flameMax);
	}
	if (type == "Cube")
	{
		cubes[cubeCount] = new Cube((Cube*)visitor);
		cubeCount++;
		if (cubeCount == cubeMax)
			expandEnemyArray(cubes, cubeMax);
	}
	if (type == "Spider")
	{
		spiders[spiderCount] = new Spider((Spider*)visitor);
		spiderCount++;
		if (spiderCount == spiderMax)
			expandEnemyArray(spiders, spiderMax);
	}
	if (type == "Ghost")
	{
		ghosts[ghostCount] = new Ghost((Ghost*)visitor);
		ghostCount++;
		if (ghostCount == ghostMax)
			expandEnemyArray(ghosts, ghostMax);
	}
	if (type == "Web")
	{
		webs[webCount] = new Web((Web*)visitor);
		webCount++;
		if (webCount == webMax)
			expandEnemyArray(webs, webMax);
	}
	if (type == "Webshot")
	{
		webShots[shotCount] = new Webshot((Webshot*)visitor);
		shotCount++;
		if (shotCount == shotMax)
			expandEnemyArray(webShots, shotMax);
	}
	if (type == "Spellbook")
	{
		spellbooks[spellbookCount] = new Spellbook((Spellbook*)visitor);
		spellbookCount++;
		if (spellbookCount == spellbookMax)
			expandEnemyArray(spellbooks, spellbookMax);
	}
	if (type == "Missile")
	{
		missiles[missileCount] = new ArcaneMissile((ArcaneMissile*)visitor);
		missileCount++;
		if (missileCount == missileMax)
			expandEnemyArray(missiles, missileMax);
	}
}