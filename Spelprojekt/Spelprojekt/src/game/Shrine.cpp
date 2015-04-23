#include "Shrine.h"

Shrine::Shrine(GameObject* object)
{
	runeTaken = false;
	this->object = object;
	if (object->returnID() == 2)
	{
		rune = FLAME;
		runeObj = new GameObject();
		runeObj->init(1);
		glm::vec3 pos = object->readPos();
		runeObj->moveTo(pos.x, pos.y, pos.z + 1);
		runeObj->scaleFactor(0.5, 0.5, 0.5);
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

void Shrine::resetRune()
{
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
		runeTaken = true;
		return rune;
	}
	return NORUNE;
}