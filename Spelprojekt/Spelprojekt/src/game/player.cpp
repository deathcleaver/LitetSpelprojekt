#include "player.h"

#include <stdio.h>


void Player::init()
{
	moveTo(0, 2);
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 2);
}

Player::~Player()
{
	delete collideRect;
}

int Player::update(UserInput* userInput, Map* map, float deltaTime)
{
	timepass += deltaTime;

	// update pos & camera using user input
	vec3 lastPos = readPos();
	vec3 tempPos = lastPos;
	float speed = 10.0f;
	int idX = 0, idY = 0;
	bool result = false;
	
	//MoveX
	if (userInput->getKeyState('A')) {
		moveTo(tempPos.x -= speed * deltaTime, tempPos.y, 0);
		dir = -1; }
	if (userInput->getKeyState('D')){
		moveTo(tempPos.x += speed * deltaTime, tempPos.y, 0);
		dir = 1; }

	//update collide rect
	collideRect->update();

	map->getChunkIndex(vec2(readPos().x, readPos().y), &idX, &idY);
	if (idX != -1 && idY != -1)
		result = map->getChunks()[idX][idY].collide(collideRect);

	if (result) //collide, move back X
	{
		tempPos.x = lastPos.x;
		moveTo(lastPos.x, tempPos.y);
		result = false;
	}

	//MoveY
	if (userInput->getKeyState('W'))
		moveTo(tempPos.x, tempPos.y += speed * deltaTime, 0);

	if (userInput->getKeyState('S'))
		moveTo(tempPos.x, tempPos.y -= speed * deltaTime, 0);

	//update collide rect
	collideRect->update();

	map->getChunkIndex(vec2(readPos().x, readPos().y), &idX, &idY);
	if (idX != -1 && idY != -1)
		result = map->getChunks()[idX][idY].collide(collideRect);

	if (result) //collide, move back Y
	{
		tempPos.y = lastPos.y;
		moveTo(tempPos.x, lastPos.y);
		result = false;
	}

	//map->getChunkIndex(vec2(readPos().x, readPos().y), &idX, &idY);
	//if (idX != -1 && idY != -1)
	//{
	//	if (map->getChunks()[idX][idY].collide(collideRect)) //collideRect->intersects()
	//	{
	//		result = true;
	//		printf("collision in chunk: %i.%i. Pos %f,%f\n", idX, idY, readPos().x, readPos().y);
	//	}
	//	else
	//		printf("no collision%f,%f\n", readPos().x, readPos().y);
	//}
	//else
	//	printf("out of map%f,%f\n", readPos().x, readPos().y);

	return 0;
}

int Player::getDir()
{
	return dir;
}