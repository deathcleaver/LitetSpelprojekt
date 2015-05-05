#ifndef LIGHT_H
#define LIGHT_H

#include "../../Align16.h"

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
	void init(float x, float y)
	{
		posX = x;
		posY = y;
		r = 1;
		g = 1;
		b = 1;
		intensity = 3;
		volume = 1;
		distance = 5;
	}

	void translate(float x, float y, float z)
	{
		posX += x;
		posY += y;
		posZ += z;
	}

	void scale(float x, float y, float z)
	{
		distance += x;
		if (distance < 0.1)
			distance = 0.1;
		intensity += y / 2;
		if (intensity < 0.1)
			intensity = 0.1;
		if (z > 0)
		{
			volume = z -1;
		}
	}

	void color(float x, float y, float z)
	{
		r += x / 5;
		g += y / 5;
		b += z / 5;

		if (r < 0)
			r = 0;
		if (r > 1)
			r = 1;

		if (g < 0)
			g = 0;
		if (g > 1)
			g = 1;

		if (b < 0)
			b = 0;
		if (b > 1)
			b = 1;
	}
};

#endif