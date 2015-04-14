#ifndef GBUFFER_H
#define GBUFFER_H

#include <gl/glew.h>

class RenderTarget
{

public:

	void init(int x, int y, bool useDepth);
	void resize(int x, int y);

	void render();

private:

	GLuint fboId;

	bool depth;
	int nrTextures;

};

#endif