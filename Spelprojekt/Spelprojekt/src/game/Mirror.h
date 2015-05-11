#ifndef MIRROR_HPP
#define MIRROR_HPP

#include "gameObject.h"
#include <gl/glew.h>

#include "../engine/GBuffer.h"

class Mirror : public GameObject
{
public:
	
	Mirror();
	~Mirror();

	void initGBuffer(Gbuffer &screenBuffer);

	bool isInitialized();

	void calculateNormal();
	void calcView();

	void render();

	Gbuffer mirrorBuffer;

	glm::mat4 viewMat;
	glm::mat4 projMat;

private:

	int sizeX, sizeY;

	bool initialized;

	glm::vec3 normal;


};

#endif
