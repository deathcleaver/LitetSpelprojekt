#ifndef LIGHT_H
#define LIGHT_H

#include "../Align16.h"

struct Light : public Align_16
{
	float posX, posY, posZ;
	float distance;
	float r, g, b;
	float intensity;

	int volume;
	float pad2;
	float pad3[2];

	Light()
	{
		posX = 0;
		posY = 0;
		posZ = 0;
		distance = 0;
		r = 0;
		g = 0;
		b = 0;
		intensity = 0;
		volume = 0;
		pad2 = 0;
		pad3[0] = 0;
		pad3[1] = 0;
	}
};

#endif