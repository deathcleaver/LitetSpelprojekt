#include "HealthPickup.h"

void HealthPickup::init(glm::vec2 pos)
{
	contentIndex = MiscID::heart;
	moveTo(pos.x, pos.y);

	myLight = new Light();
	myLight->posX = pos.x;
	myLight->posY = pos.y;
	myLight->posZ = 0.0f;
	myLight->r = 0.8f;
	myLight->g = 0.1f;
	myLight->b = 0.1f;
	myLight->distance = 15.0f;
	myLight->intensity = 2.0f;
	myLight->volume = 1;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.7f, 0.7f);
}

HealthPickup::~HealthPickup()
{
	delete myLight;
	delete collideRect;
}

int HealthPickup::update(float deltaTime)
{
	rotateTo(0, 3.141592654f*deltaTime, 0);
	return 0;
}