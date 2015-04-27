#ifndef PROGRESS_H
#define PROGRESS_H

#include <..\glm\gtc\matrix_transform.hpp>
#include <..\glm\glm.hpp>
#include "enemy.h"

struct Progress
{
	glm::vec2 pickups[4];
	int nrOfPickups = 0;
	bool batboss = false;
	bool spiderboss = false;
	bool ghostboss = false;
	void checkBossType(std::string boss)
	{
		if (boss == "Bossbat")
			batboss = true;
		if (boss == "Bossspider")
			spiderboss = true;
		if (boss == "Bossghost")
			ghostboss = true;
	}
	std::string getBossType(int index)
	{
		if (index == 0 && batboss)
			return "Bossbat";
		if (index == 1 && spiderboss)
			return "Bossspider";
		if (index == 2 && ghostboss)
			return "Bossghost";
		return "No";
	}

	void addHealth(glm::vec2 add)
	{
		pickups[nrOfPickups] = add;
		nrOfPickups++;
	}
};

#endif