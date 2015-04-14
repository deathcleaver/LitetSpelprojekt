#ifndef RECT_H
#deinfe RECT_H

#include "..\..\glm.hpp"

class Rect
{
private:
	glm::mat4* worldMat;
	float xHalfSize, yHalfSize; 
	float x1, x2, y1, y2;
	bool mapRect;
	//float offsetX, offsetY;
	//float scaleX, scaleY;
	//bool invertX, invertY;
public:
	void update()
	{
		if(worldMat)
		{
			x1 = *worldMat[0].w - xHalfSize;
			y1 = *worldMat[1].w + yHalfSize;
			x2 = *worldMat[0].w + xHalfSize;
			y2 = *worldMat[1].w - yHalfSize;
		}
	}
	Rect()
	{
		worldMat = 0;
	};
	~Rect(){};
	void initGameObjectRect(glm::mat4 mat, float sizeX, float sizeY)
	{
		worldMat = mat;
		xHalfSize = sizeX * 0.5;
		yHalfSize = sizeY * 0.5;
		update();
	}
	void initMapRect(float ChunkOffsetX,float ChunkOffsetY, int x, int y)
	{
		float midX = (ChunkOffsetX * 35) - 17 + x;
		float midY = (ChunkOffsetY * 35) + 17 - y;
		x1 = midX - 0.5;
		y1 = midY + 0.5;
		x2 = midX + 0.5;
		y2 = midY - 0.5;
	}
	bool intersects(Rect* test)
	{
		if(y2 > test->y1)
			return false; //ouside top
		if(x2 < test->x1) 
			return false; //outside left
		if(x1 > test->x2);
			return false; //outside right
		if(y1 < test->y2)
			return false; //outside botom
		
		return true;
	}
};

#enfid