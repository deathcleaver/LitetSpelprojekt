#include "Bossghost.h"
#include "../mapChunk.h"
#include "../map.h"
#include "ArcaneMissile.h"
#include "Ghost.h"
#include "Spellbook.h"

Bossghost::Bossghost(glm::vec2 firstPos)
{
	initPos = firstPos;
	topMidBot[0] = initPos.y + 4.0f;
	topMidBot[1] = initPos.y;
	topMidBot[2] = initPos.y - 4.0f;
	leftRight[0] = initPos.x - 8.0f;
	leftRight[1] = initPos.x + 8.0f;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::demon;
	health = 5;
	speed = 5.0f + GameConfig::get().configDifficulty * 2; // + (0->2)
	mirrorSpeed = 1.5f;
	audibleDistance = 10.0f;

	invulnTimer = 0.0f;
	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 2, 2);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 2.5, 4.0);
}

Bossghost::~Bossghost()
{
	delete hurtRect;
}

void Bossghost::shootMissile(glm::vec3 playerPos, Map* map, bool followPlayer)
{
	glm::vec3 pos = readPos();
	ArcaneMissile* pewpew = new ArcaneMissile(glm::vec2(readPos()));
	pewpew->setVisitor();
	glm::vec2 dir;
	if (followPlayer)
		dir = glm::vec2(playerPos) - glm::vec2(readPos());
	else
		dir = glm::vec2(initPos) - glm::vec2(readPos());
	dir.x = dir.x + (rand() % 3 - 1);
	dir.y = dir.y + (rand() % 3 - 1);
	dir = normalize(dir);
	pewpew->setDirection(dir);
	map->findNewHome(pewpew);
	delete pewpew;
}

void Bossghost::getSpooky(Map* map)
{
	glm::vec2 pos = glm::vec2(readPos());
	Ghost* spooky = new Ghost(pos);
	spooky->hit(1, true);
	spooky->setVisitor();
	map->findNewHome(spooky);
	delete spooky;
}

void Bossghost::getNerdy(Map* map)
{
	glm::vec2 pos = glm::vec2(readPos());
	Spellbook* book = new Spellbook(pos);
	book->setVisitor();
	map->findNewHome(book);
	delete book;
}

void Bossghost::init()
{
	if (!isInit)
	{
		isDying = false;
		speed = 5.0f + GameConfig::get().configDifficulty * 2;
		inMirror = true;
		worldMat = glm::mat4(1);
		scaleFactor(0.01f, 0.01f, 0.01f);
		rotateTo(0, 3.141592654f, 0);
		isInit = true;
		moveTo(initPos.x, initPos.y);
		invulnTimer = 0.0f;
		movementScale = 0.0f;
		facingRight = true;
		alive = true;
		health = 5;
		collideRect->update();
		hurtRect->update();
		Audio::getAudio().playMusic(MusicID::boss_battle_ghost);
		Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_laugh, readPos(), audibleDistance + 2, false);

		state = -1;
		stateTimer = 5.0f;
		lastPos = -1;
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

int Bossghost::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (isDying)
	{
		alive = false;
		return 1;
	}
	glm::vec3 pos = readPos();
	if (state == -1) //Spawn state
	{
		stateTimer -= 1.0f*deltaTime;
		if (stateTimer < 2.0f)
		{
			worldMat = glm::mat4(1);
			moveTo(pos.x, pos.y);
			float scaleF = 2.0 - stateTimer;
			scaleFactor(scaleF*1.5f, scaleF*1.5f, scaleF*1.5f);
			rotateTo(0, 3.141592654f, 0);
		}
		else
			moveTo(playerPos.x, playerPos.y);
		if (stateTimer < FLT_EPSILON)
		{
			state = 0;
			calcDir(-1);
		}
	}
	else if (state == 0) //Jumping out of mirror state
	{
		if (reachedDestination())
		{
			if (inMirror)
			{
				translate(0, 0, -2.0f*deltaTime);
				pos = readPos();
				if (pos.z < -2.5f)
				{
					inMirror = false;
					rotateTo(0, 3.141592654f, 0);
					Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_laugh, readPos(), audibleDistance + 2, false);
				}
			}
			else
				translate(0, 0, 2.0f*deltaTime);
			pos = readPos();
			if (pos.z > -FLT_EPSILON)
			{
				invulnTimer = 1.0f;
				state = 1;
				int posOutOfMirror = rand() % 6;
				if (posOutOfMirror == lastPos)
					posOutOfMirror = (posOutOfMirror + 1) % 6;
				lastPos = posOutOfMirror;
				calcDir(posOutOfMirror);
				getSpooky(map);
				Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_laugh, readPos(), audibleDistance + 2, false);
				ghostTimer = 12.0f;
				//ghostTimer = 0.0f;
				//ghostSpawns = 4 + GameConfig::get().configDifficulty * 2;
			}
		}
		else
			translate(dirToFly.x*speed*deltaTime, dirToFly.y*speed*deltaTime);
	}
	else if (state == 1) //Flying outside of mirror state
	{
		translate(dirToFly.x*speed*deltaTime, dirToFly.y*speed*deltaTime);
		if (reachedDestination())
		{
			state = 2;
			stateTimer = 1.0f;
			missilesLeft = 4;
		}
		ghostTimer -= 1.0f*deltaTime;
		if (ghostTimer < FLT_EPSILON)
		{
			state = 3;
			calcDir(-1);
		}

		/*ghostTimer -= 1.0f*deltaTime;
		if (ghostTimer < FLT_EPSILON && ghostSpawns > 0)
		{
			if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
			{
				getNerdy(map);
				ghostTimer = 6.0f;
			}		
			else
			{
				getSpooky(map);
				ghostTimer = 4.0f;
			}
			ghostSpawns--;

			Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_laugh, readPos(), audibleDistance + 2, false);
		}
		*/
	}
	else if (state == 2) //Missile barrage state
	{
		stateTimer -= 1.0f;
		if (stateTimer < FLT_EPSILON && missilesLeft != 0)
		{
			shootMissile(playerPos, map, false);
			missilesLeft--;
			stateTimer = 0.3f;
		}
		else if (missilesLeft == 0)
		{
			state = 1;
			int posOutOfMirror = rand() % 6;
			if (posOutOfMirror == lastPos)
				posOutOfMirror = (posOutOfMirror + 1) % 6;
			lastPos = posOutOfMirror;
			calcDir(posOutOfMirror);
		}
	}
	else if (state == 3) //Jumping into mirror state
	{
		if (reachedDestination())
		{
			if (!inMirror)
			{
				translate(0, 0, -2.0f*deltaTime);
				pos = readPos();
				if (pos.z < -2.5f)
				{
					rotateTo(0, 3.141592654f, 0);
					inMirror = true;
					Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_laugh, readPos(), audibleDistance + 2, false);
				}
			}
			else
				translate(0, 0, 2.0f*deltaTime);
			pos = readPos();
			if (pos.z > -FLT_EPSILON)
			{
				if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
					getNerdy(map);
				else
					getSpooky(map);
				Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_laugh, readPos(), audibleDistance + 2, false);
				state = 4;
				stateTimer = 1.0f;
				missilesLeft = 10;
			}
		}
		else
			translate(dirToFly.x*speed*deltaTime, dirToFly.y*speed*deltaTime);
	}
	else if (state == 4) //Flying inside mirror state
	{
		stateTimer -= 1.0f*deltaTime;
		if (stateTimer < FLT_EPSILON && missilesLeft > 0)
		{
			shootMissile(playerPos, map, true);
			shootMissile(playerPos, map, true);
			missilesLeft--;
			stateTimer = 1.0f;
		}
		else if (missilesLeft == 0)
		{
			state = 0;
			calcDir(-1);
		}
		glm::vec2 dist = glm::vec2(playerPos) - glm::vec2(pos);
		glm::vec2 normDist = normalize(dist);
		translate(normDist.x*mirrorSpeed*deltaTime, normDist.y*mirrorSpeed*deltaTime);
	}
	hurtRect->update();
	collideRect->update();
	if (invulnTimer > FLT_EPSILON)
		invulnTimer -= 1.0f*deltaTime;
	return 0;
}

bool Bossghost::isInitiated()
{
	return isInit;
}

void Bossghost::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON && !inMirror && state != 0 && state != 3)
	{
		health -= damage;
		if (health > 0)
		{
			if (health <= 4)
				speed = 7.0f;
			if (health <= 2)
				speed = 9.0f;
			invulnTimer = 1.0f;
			Debug::DebugOutput("Boss took damage \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_hurt, readPos(), audibleDistance, false);//boss_bat_hurt
			state = 3;
			calcDir(-1);
		}
		else if (alive == true)
		{
			isDying = true;
			Debug::DebugOutput("Boss is dead \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_ghost_death, readPos(), audibleDistance, false);//boss_bat_death
		}
	}
}

bool Bossghost::isBlinking()
{
	if (inMirror)
		return true;
	if (invulnTimer > 0.0f)
	{
		int check = int(invulnTimer * 10);
		if (check % 3)
		{
			return true;
		}
	}
	return false;
}

glm::vec2 Bossghost::plsGiveBossPos()
{
	return initPos;
}

void Bossghost::calcDir(int posOutOfMirror) //0 = top left, 1 = mid left, 2 = bot left, 3 = top right, 4 = mid right, 5 = bot right,  -1 = mid
{
	glm::vec2 pos = glm::vec2(readPos());
	switch (posOutOfMirror)
	{
	case -1:
		currentGoal = initPos;
		break;
	case 0:
		currentGoal.x = leftRight[0];
		currentGoal.y = topMidBot[0];
		break;
	case 1:
		currentGoal.x = leftRight[0];
		currentGoal.y = topMidBot[1];
		break;
	case 2:
		currentGoal.x = leftRight[0];
		currentGoal.y = topMidBot[2];
		break;
	case 3:
		currentGoal.x = leftRight[1];
		currentGoal.y = topMidBot[0];
		break;
	case 4:
		currentGoal.x = leftRight[1];
		currentGoal.y = topMidBot[1];
		break;
	case 5:
		currentGoal.x = leftRight[1];
		currentGoal.y = topMidBot[2];
		break;
	}

	dirToFly = currentGoal - pos;
	dirToFly = normalize(dirToFly);
}

bool Bossghost::reachedDestination()
{
	glm::vec3 pos = readPos();
	if (pos.x < currentGoal.x + 1.0f && pos.x > currentGoal.x - 1.0f &&
		pos.y < currentGoal.y + 1.0f && pos.y > currentGoal.y - 1.0f)
		return true;
	return false;
}