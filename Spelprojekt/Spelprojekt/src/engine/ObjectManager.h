#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include"AnimationObject.h"
#include <vector>
#include "../legend.h"
#include "updateAnimCheck.h"

using std::vector;

class ObjectManager
{
private:
	AnimationObject* collisionRekt;
	AnimationObject* myPlayer;
	
	vector<vector<AnimationObject*>> objects;

	void loadPlayer();
	void loadBackObjs();
	void loadMapObjs();
	void loadMiscObjs();
	void loadMonsterObjs();

	int myToUpdate;
public:
	ObjectManager();
	~ObjectManager();
	void init();

	void update(UpdateAnimCheck* animCheck);
	void setPlayerState(std::string state);
	int bind(int type, int id) const;
	int bindPlayer() const;
	void bindRekt() const;
	int nrOfWorldItems() const;
	int nrOfMonsterItems() const;
};
#endif