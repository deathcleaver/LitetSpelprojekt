#ifndef PROGRESS_H
#define PROGRESS_H

#include <..\glm\gtc\matrix_transform.hpp>
#include <..\glm\glm.hpp>
#include "enemy.h"

struct Progress
{
	bool batboss = false;
	bool spiderboss = false;
	bool scaryboss = false;
	void checkBossType(std::string boss)
	{
		if (boss == "Bossbat")
			batboss = true;
		if (boss == "Spiderboss")
			spiderboss = true;
		if (boss == "Grim")
			scaryboss = true;
	}
};

#endif