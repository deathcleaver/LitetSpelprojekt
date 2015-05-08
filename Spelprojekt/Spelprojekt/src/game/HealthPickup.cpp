#include "HealthPickup.h"

void HealthPickup::init(glm::vec2 pos)
{
	contentIndex = MiscID::heart;
	moveTo(pos.x, pos.y);

	myLight = new Light();
	myLight->posX = pos.x;
	myLight->posY = pos.y;
	myLight->posZ = 0.0f;
	myLight->r = 0.8;
	myLight->g = 0.1;
	myLight->b = 0.1;
	myLight->distance = 15.0f;
	myLight->intensity = 2.0f;
	myLight->volume = 1;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.7, 0.7);
}

HealthPickup::~HealthPickup()
{
	delete myLight;
	delete collideRect;
}

int HealthPickup::update(float deltaTime)
{
	return 0;
}