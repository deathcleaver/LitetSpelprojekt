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


	void setTeleportLocation(glm::vec2 loc, glm::vec2 chunk)
	{
		teleportLocation = loc;
		chunkLocation = chunk;
	}
	glm::vec2 getTeleportLocation()
	{
		return glm::vec2(teleportLocation.x+chunkLocation.x*35, teleportLocation.y-chunkLocation.y*35);
	}

	glm::vec2 getChunkTeleport()
	{
		return chunkLocation;
	}

	void setRect();
	Rect* getRekt()
	{
		return enterRect;
	}

private:

	int sizeX, sizeY;

	bool initialized;

	glm::vec3 normal;

	glm::vec2 teleportLocation;
	glm::vec2 chunkLocation;

	Rect* enterRect;
};

#endif
