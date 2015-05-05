#ifndef UPDATEANIMCHECK_H
#define UPDATEANIMCHECK_H
#include "../legend.h"

class UpdateAnimCheck
{
public:
	bool* enemyUpdate = new bool[EnemyID::enemy_count];
	bool* worldUpdate = new bool[WorldID::world_count];
	bool* miscUpdate = new bool[MiscID::misc_count];

	UpdateAnimCheck() 
	{
		for (int i = 0; i < EnemyID::enemy_count; i++)
		{
			enemyUpdate[i] = 0;
		}
		for (int i = 0; i < WorldID::world_count; i++)
		{
			worldUpdate[i] = 0;
		}
		for (int i = 0; i < MiscID::misc_count; i++)
		{
			miscUpdate[i] = 0;
		}
	}
	void reset()
	{
		for (int i = 0; i < EnemyID::enemy_count; i++)
		{
			enemyUpdate[i] = 0;
		}
		for (int i = 0; i < WorldID::world_count; i++)
		{
			worldUpdate[i] = 0;
		}
		for (int i = 0; i < MiscID::misc_count; i++)
		{
			miscUpdate[i] = 0;
		}
	}
	~UpdateAnimCheck()
	{
		delete[] enemyUpdate;
		delete[] worldUpdate;
		delete[] miscUpdate;
	}
};
#endif