#include "player.h"

void Player::init()
{
	worldMat = { 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1 };
	//scale
	float scale = 0.03;
	worldMat[0].x *= scale;
	worldMat[1].y *= scale;
	worldMat[2].z *= scale;
}

void Player::bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const
{
	glProgramUniformMatrix4fv(*shaderProgram, *shaderuniform, 1, false, &worldMat[0][0]);
}