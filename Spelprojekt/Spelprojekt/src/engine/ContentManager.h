#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include <string>
#include "object.h"

class ContentManager
{
private:
	Object* player = 0;
	void loadPlayer();

	Object** mapObjs = 0;
	int mapObjCount = 0;
	void loadMapObjs();

	Object** monsterObjs = 0;
	int monsterObjCount = 0;
	void loadMonsterObjs();
	
public:
	ContentManager() {};
	~ContentManager();
	void init();
	int bindPlayer() const;
	int bindMapObj(int id) const;
	int bindMonsterObj(int id) const;
};

#endif