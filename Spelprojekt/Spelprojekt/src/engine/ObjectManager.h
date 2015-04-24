#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include"AnimationObject.h"

class ObjectManager
{
private:
	AnimationObject* collisionRekt;
	AnimationObject* myPlayer;
	AnimationObject** myMapObjs;
	AnimationObject** myMonsterObjs;

	void loadPlayer();
	void loadMapObjs();
	void loadMonsterObjs();

	int myNrOfMapObjects;
	int myNrOfMonsterObjects;
	int myToUpdate;
public:
	ObjectManager();
	~ObjectManager();
	void init();

	void update();
	void setPlayerState(std::string state);
	int bindPlayer() const;
	int bindMapObj(int id) const;
	int bindMonsterObj(int id) const;
	void bindRekt() const;
	int nrOfWorldItems() const;
	int nrOfMonsterItems() const;
};
#endif