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

void GameObject::translateEDITOR(float x, float y, float z)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
	worldMat[2].w += z;
	posXfloatsave = worldMat[0].w;
	posYfloatsave = worldMat[1].w;
	posZfloatsave = worldMat[2].w;
}

void GameObject::translateSNAP(float x, float y, float z)
{
	if (posXfloatsave < -99900.0f)
		posXfloatsave = worldMat[0].w;
	if (posYfloatsave < -99900.0f)
		posYfloatsave = worldMat[1].w;
	if (posZfloatsave < -99900.0f)
		posZfloatsave = worldMat[2].w;

	posXfloatsave += x;
	posYfloatsave += y;
	posZfloatsave += z;

	worldMat[0].w = (int)posXfloatsave;
	worldMat[1].w = (int)posYfloatsave;
	worldMat[2].w = (int)posZfloatsave;

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

void GameObject::moveToXY(float x, float y)
{
	worldMat[0].w = x;
	worldMat[1].w = y;
}

void GameObject::moveToZ(float z)
{
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

void GameObject::scaleAD(float x, float y, float z)
{
	worldMat[0].x += x;
	worldMat[1].y += y;
	worldMat[2].z += z;
}

void GameObject::scaleFactorADsnap(float x, float y, float z)
{
	worldMat[0].x += x;
	worldMat[0].x = (int)worldMat[0].x;
	worldMat[1].y += y;
	worldMat[1].y = (int)worldMat[1].y;
	worldMat[2].z += z;
	worldMat[2].z = (int)worldMat[2].z;
}

int GameObject::update(float deltaTime)
{
	return 0;
}

int GameObject::returnID() const
{
	return contentIndex;
}