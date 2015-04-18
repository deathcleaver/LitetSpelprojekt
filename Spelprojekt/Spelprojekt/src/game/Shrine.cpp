#include "Shrine.h"

Shrine::Shrine(GameObject* object)
{
	this->object = object;
	if (object->returnID() == 2)
	{
		powerUp = 1;
	}
	collision = new Rect();
	collision->initGameObjectRect(object->getWorldMat(), 1.0f, 2.0f);
}

Shrine::~Shrine()
{
	delete collision;
}

Rect* Shrine::getRekt()
{
	return collision;
}

glm::vec2 Shrine::getPos()
{
	return glm::vec2(object->readPos());

}