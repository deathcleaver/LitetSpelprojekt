#ifndef LIGHT_H
#define LIGHT_H

#include "../Align16.h"

struct Light : public Align_16
{
	float posX, posY, posZ;
	float pad1;
	float r, g, b;
	float pad2;
	float distance;
	float intensity;
	float pad3[2];

};

#endif