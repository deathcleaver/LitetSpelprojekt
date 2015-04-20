#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include <string>
#include "object.h"
#include "ObjectManager.h"


class ContentManager
{
private:
	ObjectManager* objectManager;
	void loadObjManager();
	Object* player = 0;
	void loadPlayer();

	Object** mapObjs = 0;
	int mapObjCount = 0;
	void loadMapObjs();

	Object** monsterObjs = 0;
	int monsterObjCount = 0;
	void loadMonsterObjs();

	Object** guiObjs = 0;
	int guiCount = 0;
	void loadGUIobjs();
	
public:
	ContentManager() {};
	~ContentManager();
	void init();
	void update();
	int bindPlayer() const;
	int bindMapObj(int id) const;
	int bindMonsterObj(int id) const;
	void bindGUItex(int id) const;
	void bindGUIvert() const;
	Object** gui(); //used to auto resize texture objects in GUI
	void setPlayerState(std::string state);
};

#endif