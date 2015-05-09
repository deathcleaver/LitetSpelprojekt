#include"AnimationObject.h"

AnimationObject::AnimationObject()
{
	myBaseObjects = nullptr;
	myAnimObject = nullptr;

	myNrOfBaseObjects = 0;

	myWeight = -1.0f;
	mySpeed = -1.0f;
	myDirection = 1;
	myAnimPoint1 = myAnimPoint2 = 0;
}

AnimationObject::AnimationObject(Object* aBase1, Object* aBase2, float aWeight, float aSpeed)
{
	myBaseObjects = new Object*[2];
	myNrOfBaseObjects = 2;
	myBaseObjects[0] = aBase1;
	myBaseObjects[1] = aBase2;

	myWeight = aWeight;
	mySpeed = aSpeed;
	myDirection = 1;

	myAnimObject = new Object(*myBaseObjects[0]);
	myAnimPoint1 = 0;
	myAnimPoint2 = 1;
}

AnimationObject::AnimationObject(Object** someBases, std::string pathVert, int aNrOfBases, float aWeight, float aSpeed)
{
	myNrOfBaseObjects = aNrOfBases;

	myBaseObjects = someBases;

	myWeight = aWeight;
	mySpeed = aSpeed;
	myDirection = 1;

	if (myNrOfBaseObjects == 1)
	{
		myAnimObject = myBaseObjects[0];
	}
	else
	{
		myAnimObject = new Object(pathVert, "", myBaseObjects[0], false, true);
	}
	myAnimPoint1 = myAnimPoint2 = 0;
}

AnimationObject::AnimationObject(Object* aBase)
{
	myNrOfBaseObjects = 1;

	myBaseObjects = new Object*[myNrOfBaseObjects];
	
	myBaseObjects[0] = aBase;
	myAnimObject = aBase;

	myWeight = 0.0f;
	mySpeed = 0.0f;
	myDirection = 1;
	myAnimPoint1 = myAnimPoint2 = 0;
}

AnimationObject::AnimationObject(std::string pathVert, std::string pathTex)
{
	myNrOfBaseObjects = 1;

	myBaseObjects = new Object*[myNrOfBaseObjects];

	myBaseObjects[0] = new Object(pathVert, pathTex);
	myAnimObject = myBaseObjects[0];

	myWeight = 0.0f;
	mySpeed = 0.0f;
	myDirection = 1;
	myAnimPoint1 = myAnimPoint2 = 0;
}

AnimationObject::~AnimationObject()
{
	for (int i = 0; i < myNrOfBaseObjects; i++)
	{
		delete myBaseObjects[i];
		myBaseObjects[i] = 0;
	}
	delete[] myBaseObjects;

	if (myNrOfBaseObjects > 1)
		delete myAnimObject;
}

Object* AnimationObject::getAnimObject()
{
	return myAnimObject;
}

float AnimationObject::getWeight()
{
	return myWeight;
}

float AnimationObject::getSpeed()
{
	return mySpeed;
}

void AnimationObject::setAnimObject(Object* aObject)
{
	if (myAnimObject != nullptr)
		delete myAnimObject;

	myAnimObject = new Object(*aObject);
}

void AnimationObject::setWeight(float aWeight)
{
	myWeight = aWeight;
}

void AnimationObject::setSpeed(float aSpeed)
{
	mySpeed = aSpeed;
}

void AnimationObject::setDirection(int aDirection)
{
	myDirection = aDirection;
}

void AnimationObject::setAnimPoints(int aPoint1, int aPoint2)
{
	myAnimPoint1 = aPoint1;
	myAnimPoint2 = aPoint2;
}

void AnimationObject::update()
{
	if (myAnimPoint1 != myAnimPoint2 && myNrOfBaseObjects > 1)
	{
		//If our target and our base is the same, it is unecessary to update the animTarget, since it would be the same

		updateWeight();

		float weightDif = 1 - myWeight;
		if (weightDif > 1.0f) weightDif = 1.0f;
		if (weightDif < 0.0f) weightDif = 0.0f;

		float sum_weight = myWeight + weightDif;

		float anim_factor = myWeight / sum_weight;
		float normal_factor = weightDif / sum_weight;

		std::vector<Object::TriangleVertex> combVert;

		Object::TriangleVertex temp;
		Object::TriangleVertex temp2;
		for (unsigned int i = 0; i < myBaseObjects[myAnimPoint1]->vert.size() ; i++)
		{
			combVert.push_back(Object::TriangleVertex());

			temp.x = myBaseObjects[myAnimPoint1]->vert[i].x * normal_factor;
			temp.y = myBaseObjects[myAnimPoint1]->vert[i].y * normal_factor;
			temp.z = myBaseObjects[myAnimPoint1]->vert[i].z * normal_factor;
			temp.u = myBaseObjects[myAnimPoint1]->vert[i].u * normal_factor;
			temp.v = myBaseObjects[myAnimPoint1]->vert[i].v * normal_factor;


			temp2.x = myBaseObjects[myAnimPoint2]->vert[i].x * anim_factor;
			temp2.y = myBaseObjects[myAnimPoint2]->vert[i].y * anim_factor;
			temp2.z = myBaseObjects[myAnimPoint2]->vert[i].z * anim_factor;
			temp2.u = myBaseObjects[myAnimPoint2]->vert[i].u * anim_factor;
			temp2.v = myBaseObjects[myAnimPoint2]->vert[i].v * anim_factor;

			combVert[i].x = temp.x + temp2.x;
			combVert[i].y = temp.y + temp2.y;
			combVert[i].z = temp.z + temp2.z;
			combVert[i].u = temp.u + temp2.u;
			combVert[i].v = temp.v + temp2.v;
		}
		myAnimObject->updateVAO(combVert, myAnimObject->Indices);
	}
	if (myAnimPoint2 == myAnimPoint1 && myNrOfBaseObjects > 1)
	{
		//If we try to interpolate between the same two verts, we simply set the Rendered vertexbuffer to the same, this is also to make sure that we can stop an animationcycle
		myAnimObject->updateVAO(myBaseObjects[myAnimPoint2]->vert, myBaseObjects[myAnimPoint2]->Indices);
	}
	bindAnimObject();
}

void AnimationObject::bindAnimObject() const
{
	myAnimObject->bind();
}

void AnimationObject::updateWeight()
{
	if (myNrOfBaseObjects > 1)
	{
		if (myWeight >= 1.0f)
			myDirection = -1;
		else if (myWeight <= 0.0f)
			myDirection = 1;

		myWeight += mySpeed * myDirection;
	}
}

int AnimationObject::getFaces() const
{
	return myAnimObject->getFaces();
}