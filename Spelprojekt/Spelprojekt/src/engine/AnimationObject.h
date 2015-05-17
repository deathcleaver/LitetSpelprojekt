#ifndef ANIMATIONOBJECT_H
#define ANIMATIONOBJECT_H

#include"object.h"
#include <..\glm\glm.hpp>

class AnimationObject
{
private:
	Object** myBaseObjects;//Always 2 big
	Object* myAnimObject;

	int myNrOfBaseObjects;

	int myAnimPoint1;
	int myAnimPoint2;

	bool isCycling = true;

	float myWeight; //between 0 and 1, decides where in the animation we are
	float mySpeed; //decides how fast weight is altered
	int myDirection;

public:
	AnimationObject();
	AnimationObject(Object* aBase1, Object* aBase2, float aWeight = 0.0f, float aSpeed = 0.05f, bool cycle = true);
	AnimationObject(Object** someBases, std::string pathVert, int aNrOfBases, float aWeight = 0.0f, float aSpeed = 0.05f);
	AnimationObject(Object* aBase);
	AnimationObject(std::string pathVert, std::string pathTex);
	~AnimationObject();

	Object* getAnimObject();
	float getWeight();
	float getSpeed();

	int getFaces() const;

	void setAnimObject(Object* aObject);
	void setWeight(float aWeight);
	void setSpeed(float aSpeed);
	void setDirection(int aDirection);
	void setAnimPoints(int aPoint1, int aPoint2);

	void update();
	void bindAnimObject() const;

	void updateWeight();
};
#endif