#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include <string>
#include "object.h"
#include"animation.h"

class ContentManager
{
private:
	Animation* player;
	void loadPlayer();

	Object** mapObjs;
	int mapObjCount = 0;
	void loadMapObjs();
public:
	ContentManager() {};
	~ContentManager();
	void init();
	int bindPlayer() const;
	int bindMapObj(int id) const;

};

#endif