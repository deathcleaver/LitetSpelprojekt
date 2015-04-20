#ifndef ANIMATIONOBJECT_H
#define ANIMATIONOBJECT_H

#include"object.h"

class AnimationObject
{
private:
	Object** myBaseObjects;//Always 2 big
	Object* myAnimObject;

	int myNrOfBaseObjects;

	float myWeight; //between 0 and 1, decides where in the animation we are
	float mySpeed; //decides how fast weight is altered
	int myDirection;

public:
	AnimationObject();
	AnimationObject(Object* aBase1, Object* aBase2, float aWeight = 0.0f, float aSpeed = 0.05f);
	AnimationObject(Object** someBases, int aNrOfBases, float aWeight = 0.0f, float aSpeed = 0.05f);
	AnimationObject(Object* aBase);
	~AnimationObject();

	Object* getAnimObject();
	float getWeight();
	float getSpeed();

	int getFaces();

	void setAnimObject(Object* aObject);
	void setWeight(float aWeight);
	void setSpeed(float aSpeed);

	void update(int aTargetPos = 1, int aBasePos = 0);
	void bindAnimObject();

	void updateWeight();
};
#endif