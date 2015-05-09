#include "Shrine.h"

Shrine::Shrine(GameObject* object, MiscID runetype)
{
	lightForPlayer = new RuneLights();
	runeMove = 0.0f;
	runeUp = true;
	runeTaken = false;
	this->object = object;
	glm::vec3 pos = object->readPos();

	myLight = new Light();
	myLight->posX = pos.x;
	myLight->posY = pos.y;
	myLight->posZ = pos.z + 0.5f;

	switch (runetype)
	{
	case ((MiscID)0) :
		rune = (MiscID)0;
		runeObj = 0;
		break;
	case MiscID::rune_range:
		rune = MiscID::rune_range;
		runeObj = new GameObject();
		runeObj->init(MiscID::rune_range);
		runeObj->moveTo(pos.x, pos.y + 0.1f, pos.z + 0.5f);
		myLight->r = 0.5f;
		myLight->g = 0.25f;
		myLight->b = 0.01f;
		break;
	case MiscID::rune_damage:
		rune = MiscID::rune_damage;
		runeObj = new GameObject();
		runeObj->init(MiscID::rune_damage);
		runeObj->moveTo(pos.x, pos.y + 0.1f, pos.z + 0.5f);
		myLight->r = 0.0f;
		myLight->g = 0.5f;
		myLight->b = 0.5f;
		break;
	case MiscID::rune_shield:
		rune = MiscID::rune_shield;
		runeObj = new GameObject();
		runeObj->init(MiscID::rune_shield);
		runeObj->moveTo(pos.x, pos.y + 0.1f, pos.z + 0.5f);
		myLight->r = 0.0f;
		myLight->g = 0.5f;
		myLight->b = 0.0f;
		break;
	}

	myLight->intensity = 2.0f;
	myLight->distance = 15.0f;
	myLight->volume = 1;

	collision = new Rect();
	collision->initGameObjectRect(object->getWorldMat(), 1.0f, 2.0f);
}

Shrine::~Shrine()
{
	delete collision;
	delete object;
	delete runeObj;
	delete lightForPlayer;
	delete myLight;
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

MiscID Shrine::getRune()
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
	return (MiscID)0;
}

void Shrine::update(float deltaTime)
{
	glm::vec3 runePos = runeObj->readPos();
	if (runeMove > 0.5f)
		runeUp = false;
	if (runeMove < -0.5f)
		runeUp = true;
	if (runeUp)
	{
		runeObj->moveTo(runePos.x, runePos.y + 0.1f*deltaTime);
		runeMove += 1.0f*deltaTime;
	}
	else
	{
		runeObj->moveTo(runePos.x, runePos.y - 0.1f*deltaTime);
		runeMove -= 1.0f*deltaTime;
	}
	runeObj->rotateTo(0, 1.0f*deltaTime, 0);
}