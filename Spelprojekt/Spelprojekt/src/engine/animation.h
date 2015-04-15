#ifndef ANIMATION_H
#define ANIMATION_H

#include"object.h"

class Animation
{
private:
	Object** myObjects;
	int myNrOfObjects;

	GLuint vertexData = 0;
	GLuint indexBuffer = 0;
	GLuint vertexAttribute = 0;

	int faceCount;
	float myAnimWeight;
public:
	Animation();
	Animation(Object** someObjects, int aNrOfObjects, float aAnimWeight);
	~Animation();

	void bindVertexArrays(int aAnimTargetPos = 0, int aBasePos = 0);
	Object** getObjects(int& aNrOfObjects);
};
#endif