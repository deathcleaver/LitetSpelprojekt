#include "gameObject.h"

void GameObject::bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const
{
	glProgramUniformMatrix4fv(*shaderProgram, *shaderuniform, 1, false, &worldMat[0][0]);
}

void GameObject::rotate(float x, float y, float z)
{
	//float rotx = toRad * x;
	//float roty = toRad * y;
	//float rotz = toRad * z;
	float rotx =  x;
	float roty =  y;
	float rotz =  z;


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

void GameObject::translate(float x, float y, float z)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
	worldMat[2].w += z;
	pos.x += x;
	pos.y += y;
	pos.z += z;

}

void GameObject::scaleUniform(float val)
{
	worldMat[0].x += val;
	worldMat[1].y += val;
	worldMat[2].z += val;
	scale += val;
}