#include "Shrine.h"

Shrine::Shrine(GameObject* object, int runetype)
{
	runeMove = 0.0f;
	runeUp = true;
	runeTaken = false;
	this->object = object;
	glm::vec3 pos = object->readPos();
	switch (runetype)
	{
	case 0 :
		rune = FLAME;
		runeObj = new GameObject();
		runeObj->init(4);
		runeObj->moveTo(pos.x, pos.y + 0.1, pos.z + 0.5);
		break;
	case 1 :
		rune = SPARK;
		runeObj = new GameObject();
		runeObj->init(5);
		runeObj->moveTo(pos.x, pos.y + 0.1, pos.z + 0.5);
		break;
	case 2 :
		rune = FORCE;
		runeObj = new GameObject();
		runeObj->init(6);
		runeObj->moveTo(pos.x, pos.y + 0.1, pos.z + 0.5);
		break;
	case 3 :
		rune = STOMP;
		runeObj = new GameObject();
		runeObj->init(4);
		runeObj->moveTo(pos.x, pos.y + 0.1, pos.z + 0.5);
		break;
	}
	collision = new Rect();
	collision->initGameObjectRect(object->getWorldMat(), 1.0f, 2.0f);
}

Shrine::~Shrine()
{
	delete collision;
	delete object;
	delete runeObj;
}

void Shrine::giveLight(Light* l)
{
	myLight = l;
}

void Shrine::resetRune()
{
	if (myLight)
	{
		myLight->intensity = 2.0f;
		myLight->distance = 15.0f;
	}
	runeTaken = false;
}

Rect* Shrine::getRekt()
{
	return collision;
}

glm::vec2 Shrine::getPos()
{
	return glm::vec2(object->readPos());
}

int Shrine::getRune()
{
	if (!runeTaken)
	{
		if (myLight)
		{
			myLight->intensity = 0.0f;
			myLight->distance = 0.0f;
		}
		runeTaken = true;
		return rune;
	}
	return NORUNE;
}

void Shrine::update(float deltaTime)
{
	glm::vec3 runePos = runeObj->readPos();
	if (runeMove > 0.5f)
		runeUp = false;
	if (runeMove < -0.5)
		runeUp = true;
	if (runeUp)
	{
		runeObj->moveTo(runePos.x, runePos.y + 0.1*deltaTime);
		runeMove += 1.0*deltaTime;
	}
	else
	{
		runeObj->moveTo(runePos.x, runePos.y - 0.1*deltaTime);
		runeMove -= 1.0*deltaTime;
	}
	runeObj->rotateTo(0, 1.0*deltaTime, 0);
}