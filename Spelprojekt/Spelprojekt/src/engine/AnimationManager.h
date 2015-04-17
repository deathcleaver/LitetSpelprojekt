#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include"AnimationObject.h"

class AnimationManager
{
private:
	AnimationObject* myPlayer;
	AnimationObject** myAnimatedObjects;

	int myNrOfObjects;
	int myToUpdate;
public:
	AnimationManager();
	~AnimationManager();
	void init();

	void update();
	int bindPlayer() const;
	int bindMapObj(int id) const;
};
#endif