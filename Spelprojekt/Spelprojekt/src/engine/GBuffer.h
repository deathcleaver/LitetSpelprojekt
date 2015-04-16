#ifndef GBUFFER_H
#define GBUFFER_H

#include "RenderTarget.h"

class Gbuffer : public RenderTarget
{
public:

	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void resize(int x, int y);

	void bind(GLuint index);

	void render();

	GLuint* shaderPtr;

private:

	void generate(int x, int y);

	RenderTarget* rTexture;

};

#endif