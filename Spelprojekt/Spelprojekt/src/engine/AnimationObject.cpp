#include"AnimationObject.h"

AnimationObject::AnimationObject()
{
	myBaseObjects = nullptr;
	myAnimObject = nullptr;

	myNrOfBaseObjects = 0;

	myWeight = -1.0f;
	mySpeed = -1.0f;
	myDirection = 1;
}

AnimationObject::AnimationObject(Object* aBase1, Object* aBase2, float aWeight, float aSpeed)
{
	myBaseObjects = new Object*[2];
	myNrOfBaseObjects = 2;
	myBaseObjects[0] = new Object(*aBase1);
	myBaseObjects[1] = new Object(*aBase2);

	myWeight = aWeight;
	mySpeed = aSpeed;
	myDirection = 1;

	myAnimObject = new Object(*myBaseObjects[0]);
}

AnimationObject::AnimationObject(Object** someBases, int aNrOfBases, float aWeight, float aSpeed)
{
	myNrOfBaseObjects = aNrOfBases;

	myBaseObjects = new Object*[myNrOfBaseObjects];

	for (int i = 0; i < myNrOfBaseObjects; i++)
	{
		myBaseObjects[i] = new Object(*someBases[i]);
	}

	myWeight = aWeight;
	mySpeed = aSpeed;
	myDirection = 1;

	myAnimObject = new Object(*myBaseObjects[0]);
}

AnimationObject::AnimationObject(Object* aBase)
{
	myNrOfBaseObjects = 1;

	myBaseObjects = new Object*[myNrOfBaseObjects];
	
	myBaseObjects[0] = new Object(*aBase);
	myAnimObject = new Object(*aBase);

	myWeight = 0.0f;
	mySpeed = 0.0f;
}

AnimationObject::~AnimationObject()
{
	delete myAnimObject;

	for (int i = 0; i < myNrOfBaseObjects; i++)
	{
		delete myBaseObjects[i];
	}
	delete[] myBaseObjects;
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

void AnimationObject::update(int aTargetPos, int aBasePos)
{
	if (aTargetPos != aBasePos && myNrOfBaseObjects > 1)
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
		for (int i = 0; i < myBaseObjects[aBasePos]->count * 3; i++)
		{
			combVert.push_back(Object::TriangleVertex());

			temp.x = myBaseObjects[aBasePos]->vert[i].x * normal_factor;
			temp.y = myBaseObjects[aBasePos]->vert[i].y * normal_factor;
			temp.z = myBaseObjects[aBasePos]->vert[i].z * normal_factor;
			temp.u = myBaseObjects[aBasePos]->vert[i].u * normal_factor;
			temp.v = myBaseObjects[aBasePos]->vert[i].v * normal_factor;


			temp2.x = myBaseObjects[aTargetPos]->vert[i].x * anim_factor;
			temp2.y = myBaseObjects[aTargetPos]->vert[i].y * anim_factor;
			temp2.z = myBaseObjects[aTargetPos]->vert[i].z * anim_factor;
			temp2.u = myBaseObjects[aTargetPos]->vert[i].u * anim_factor;
			temp2.v = myBaseObjects[aTargetPos]->vert[i].v * anim_factor;

			combVert[i].x = temp.x + temp2.x;
			combVert[i].y = temp.y + temp2.y;
			combVert[i].z = temp.z + temp2.z;
			combVert[i].u = temp.u + temp2.u;
			combVert[i].v = temp.v + temp2.v;
		}
		myAnimObject->updateVAO(combVert, myAnimObject->Indices);
	}
	if (aTargetPos == aBasePos && myNrOfBaseObjects > 1)
	{
		//If we try to interpolate between the same two verts, we simply set the Rendered vertexbuffer to the same, this is also to make sure that we can stop an animationcycle
		myAnimObject->updateVAO(myBaseObjects[aTargetPos]->vert, myBaseObjects[aTargetPos]->Indices);
	}
	bindAnimObject();
}

void AnimationObject::bindAnimObject()
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

int AnimationObject::getFaces()
{
	return myAnimObject->getFaces();
}