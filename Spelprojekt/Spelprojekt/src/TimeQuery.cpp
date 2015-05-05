#include "TimeQuery.h"

struct TimeQuery
{
	bool terminated;
	std::string name;
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point stopTime;
};

std::vector<TimeQuery> querry;

void resetQuery()
{
	querry.clear();
}


int startTimer(std::string name)
{
	TimeQuery t;
	t.terminated = false;
	t.name = name;
	t.startTime = std::chrono::high_resolution_clock::now();
	
	int index = querry.size();
	
	querry.push_back(t);

	return index;
}

void stopTimer(int index)
{
	querry[index].stopTime = std::chrono::high_resolution_clock::now();
	querry[index].terminated = true;
}

std::string getQueryResult()
{
	std::string result = "TimeQuerry\n";

	for (int i = 0; i < querry.size(); i++)
	{
		TimeQuery t = querry[i];
		result += t.name + ": " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(t.stopTime - t.startTime).count()) + " microsecond\n";
	}

	return result;
}

void terminateQuery()
{
	for (int i = 0; i < querry.size(); i++)
	{
		if (!querry[i].terminated)
		{
			querry[i].stopTime = std::chrono::high_resolution_clock::now();
			querry[i].terminated = true;
		}
	}
}