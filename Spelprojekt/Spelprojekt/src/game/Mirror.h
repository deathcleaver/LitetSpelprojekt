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

	void setTeleportLocation(glm::vec2 loc)
	{
		teleportLocation = loc;
	}
	glm::vec2 getTeleportLocation()
	{
		return teleportLocation;
	}

private:

	int sizeX, sizeY;

	bool initialized;

	glm::vec3 normal;

	glm::vec2 teleportLocation;
};

#endif
