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

	Object** guiObjs = 0;
	int guiCount = 0;
	void loadGUIobjs();
	
public:
	ContentManager() {};
	~ContentManager();
	void init();
	void update();
	void bindRekt() const;
	int bindPlayer() const;
	int bindMapObj(int id) const;
	int bindMonsterObj(int id) const;
	void bindGUItex(int id) const;
	void bindGUIvert() const;
	Object** gui(); //used to auto resize texture objects in GUI
	void setPlayerState(std::string state);
};

#endif