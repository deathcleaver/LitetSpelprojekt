#ifndef RECT_H
#define RECT_H

#include <..\glm\gtc\matrix_transform.hpp>
#include <..\glm\glm.hpp>

class Rect
{
private:
	glm::mat4* worldMat;
	float xHalfSize, yHalfSize;
	float x1, x2, y1, y2;
	int type;
	//float offsetX, offsetY;
	//float scaleX, scaleY;
	//bool invertX, invertY;
public:
	void update()
	{
		if (worldMat)
		{
			x1 = (*worldMat)[0].w - xHalfSize;
			y1 = (*worldMat)[1].w + yHalfSize;
			x2 = (*worldMat)[0].w + xHalfSize;
			y2 = (*worldMat)[1].w - yHalfSize;
		}
	}
	
	Rect()
	{
		worldMat = 0;
	};
	~Rect(){};

	void initGameObjectRect(glm::mat4* mat, float sizeX, float sizeY)
	{
		worldMat = mat;
		xHalfSize = sizeX * 0.5f;
		yHalfSize = sizeY * 0.5f;
		update();
	}

	void initMapRect(float ChunkOffsetX, float ChunkOffsetY, int x, int y, int type)
	{
		this->type = type;
		float midX = (ChunkOffsetX * 35) - 17 + x;
		float midY = -(ChunkOffsetY * 35) + 17 - y;
		x1 = midX - 0.5f;
		y1 = midY + 0.5f;
		x2 = midX + 0.5f;
		y2 = midY - 0.5f;
	}
	
	bool intersects(Rect* test)
	{
		if (y2 > test->y1)
			return false; //ouside top
		if (x2 < test->x1)
			return false; //outside left
		if (x1 > test->x2)
			return false; //outside right
		if (y1 < test->y2)
			return false; //outside botom

		return true;
	}

	bool intersectsPoint(float testx1, float testy1)
	{
		if (y2 > testy1)
			return false; //ouside top
		if (x2 < testx1)
			return false; //outside left
		if (x1 > testx1)
			return false; //outside right
		if (y1 < testy1)
			return false; //outside botom
		return true;
	}
	
	//mapchunk collide usage
	void readData(int* xMidRef, int* yMidRef, int* halfX, int* halfY)
	{
		*xMidRef = (int)(x1 + xHalfSize + 17.5f) % 35;
		*yMidRef = (int)(y1 - yHalfSize - 17.5f) % 35;
		*yMidRef *= -1;
		*halfX = (int)(xHalfSize + 1);
		*halfY = (int)(yHalfSize + 1);
	}
};

#endif