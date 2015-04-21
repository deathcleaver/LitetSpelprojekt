#ifndef LIGHT_H
#define LIGHT_H

#include "../Align16.h"

struct Light : public Align_16
{
	float posX, posY, posZ;
	float distance;
	float r, g, b;
	float intensity;
};

#endif