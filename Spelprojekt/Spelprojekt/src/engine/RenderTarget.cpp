#include "RenderTarget.h"

void RenderTarget::init(int x, int y, int nrTex, bool depth)
{
	this->depth = depth;
	glGenTextures(1, &targetId);
	resize(x, y);
	
}

RenderTarget::~RenderTarget()
{
	glDeleteTextures(1, &targetId);
}

void RenderTarget::resize(int x, int y)
{
	if (x == 0)
	{
		x = 1;
	}
	if (y == 0)
	{
		y = 1;
	}

	glBindTexture(GL_TEXTURE_2D, targetId);
	if (depth)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, x, y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, x, y, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

}

GLuint RenderTarget::getTargetId() const
{
	return targetId;
}

void RenderTarget::bind(GLuint target)
{

}

void RenderTarget::render()
{

}