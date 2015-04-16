#include"animation.h"

Animation::Animation()
{
	myObjects = nullptr;
	myNrOfObjects = 0;
	myAnimWeight = 0;
}

Animation::Animation(Object** someObjects, int aNrOfObjects, float aAnimWeight)
{
	myNrOfObjects = aNrOfObjects;

	myObjects = someObjects;

	myAnimWeight = aAnimWeight;

	glGenBuffers(1, &vertexData);

	glGenBuffers(1, &indexBuffer);

	glGenVertexArrays(1, &vertexAttribute);
}

Animation::~Animation()
{
	for (int i = 0; i < myNrOfObjects; i++)
	{
		delete myObjects[i];
	}
	delete[] myObjects;
}

void Animation::bindVertexArrays(int aAnimTargetPos, int aBasePos)
{
	//binder base
	if (aAnimTargetPos == aBasePos)
	{
		myObjects[aBasePos]->bind();
		vertexData = myObjects[aBasePos]->vertexData;
		indexBuffer = myObjects[aBasePos]->indexBuffer;
		vertexAttribute = myObjects[aBasePos]->vertexAttribute;
	}
	else
	{


		//Do this for myAnimWeight when it's assigned a value
		float weightDif = 1 - myAnimWeight;
		if (weightDif > 1.0f) weightDif = 1.0f;
		if (weightDif < 0.0f) weightDif = 0.0f;

		float sum_weight = myAnimWeight + weightDif;

		float anim_factor = myAnimWeight / sum_weight;
		float normal_factor = weightDif / sum_weight;

		struct TriangleVertex
		{
			float x, y, z;
			float u, v;
		};

		std::vector<TriangleVertex> combVert;
		std::vector<GLushort> indices;

		indices = myObjects[aBasePos]->Indices;

		for (int i = 0; i < myObjects[aBasePos]->count*3; i++)
		{
			combVert.push_back(TriangleVertex());
			TriangleVertex temp;
			temp.x = myObjects[aBasePos]->vert[i].x *normal_factor;
			temp.y = myObjects[aBasePos]->vert[i].y *normal_factor;
			temp.z = myObjects[aBasePos]->vert[i].z *normal_factor;
			temp.u = myObjects[aBasePos]->vert[i].u *normal_factor;
			temp.v = myObjects[aBasePos]->vert[i].v *normal_factor;

			TriangleVertex temp2;
			temp2.x = myObjects[aAnimTargetPos]->vert[i].x * anim_factor;
			temp2.y = myObjects[aAnimTargetPos]->vert[i].y * anim_factor;
			temp2.z = myObjects[aAnimTargetPos]->vert[i].z * anim_factor;
			temp2.u = myObjects[aAnimTargetPos]->vert[i].u * anim_factor;
			temp2.v = myObjects[aAnimTargetPos]->vert[i].v * anim_factor;

			combVert[i].x = temp.x +temp2.x;
			combVert[i].y = temp.y +temp2.y;
			combVert[i].z = temp.z +temp2.z;
			combVert[i].u = temp.u +temp2.u;
			combVert[i].v = temp.v +temp2.v;
			/*
			//combVert[i] = temp * normal_factor + temp2 * anim_factor;
			float *ptrCast = (float*)&temp;
			float *ptrCast2 = (float*)&temp2;
			float *target = (float*)&combVert[i];
			for (size_t i = 0; i < 5; i++)
			{
				target[i] = ptrCast[i] * normal_factor + ptrCast2[i] * anim_factor;
			}*/
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertexData);
		glBufferData(GL_ARRAY_BUFFER, sizeof(combVert[0])* combVert.size(), &combVert[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * myObjects[aBasePos]->count * 3, &indices[0], GL_STATIC_DRAW);
		faceCount = myObjects[aBasePos]->count;
		glEnableVertexAttribArray(indexBuffer);

		glGenVertexArrays(1, &vertexAttribute);
		glBindVertexArray(vertexAttribute);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(0));
		//uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 3));
	}
}

Object** Animation::getObjects(int& aNrOfObjects)
{
	aNrOfObjects = myNrOfObjects;

	return myObjects;
}

void Animation::bind()
{
	//haaaaaaxing :3
	myObjects[0]->bind();
	bindVertexArrays(1,0);
	glBindVertexArray(vertexAttribute);
	glBindBuffer(GL_ARRAY_BUFFER, vertexData);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

int Animation::getFaces()
{
	return faceCount;
}

void Animation::updateWeight()
{
	if (myAnimWeight >= 1.0f)
	{
		weightIncrease = false;
	}
	if (myAnimWeight <= 0.0f)
	{
		weightIncrease = true;
	}

	if (weightIncrease)
	{
		myAnimWeight += 0.002f;
	}
	else
	{
		myAnimWeight -= 0.002f;
	}

}