#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include <string>

#include "object.h"

class ContentManager
{
private:
	Object* player;
public:
	ContentManager() {};
	void init();
	void loadPlayer();
	int bindPlayer() const;

	//void loadMapObj(int id)
	//void bindMapObj(int id)

};

#endif