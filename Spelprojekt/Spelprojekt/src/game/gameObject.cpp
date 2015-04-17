#include "gameObject.h"

int GameObject::bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const
{
	glProgramUniformMatrix4fv(*shaderProgram, *shaderuniform, 1, false, &worldMat[0][0]);
	return contentIndex;
}

void GameObject::init(int idi)
{
	contentIndex = idi;
}

glm::mat4* GameObject::getWorldMat()
{
	return &worldMat;
}

void GameObject::rotate(float x, float y, float z)
{
	//float rotx = toRad * x;
	//float roty = toRad * y;
	//float rotz = toRad * z;
	float rotx =  x;
	float roty =  y;
	float rotz =  z;

	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//rot z

	//rot x

	//rot Y
	worldMat *= glm::mat4(cos(roty), 0.0f, -sin(roty), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(roty), 0.0f, cos(roty), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::translate(float x, float y)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
}

void GameObject::translate(float x, float y, float z)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
	worldMat[2].w += z;
}

void GameObject::moveTo(glm::vec3 target)
{
	worldMat[0].w = target.x;
	worldMat[1].w = target.y;
	worldMat[2].w = target.z;
}

void GameObject::moveTo(float x, float y)
{
	worldMat[0].w = x;
	worldMat[1].w = y;
}

void GameObject::moveTo(float x, float y, float z)
{
	worldMat[0].w = x;
	worldMat[1].w = y;
	worldMat[2].w = z;
}

glm::vec3 GameObject::readPos() const
{
	return glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);
}

void GameObject::scaleUniformAD(float val)
{
	worldMat[0].x += val;
	worldMat[1].y += val;
	worldMat[2].z += val;
}

void GameObject::scaleUniformFactor(float val)
{
	worldMat[0].x *= val;
	worldMat[1].y *= val;
	worldMat[2].z *= val;
}

void GameObject::scaleFactor(float x, float y, float z)
{
	worldMat[0].x *= x;
	worldMat[1].y *= y;
	worldMat[2].z *= z;
}

int GameObject::update(float deltaTime)
{
	return 0;
}

int GameObject::returnID()
{
	return contentIndex;
}