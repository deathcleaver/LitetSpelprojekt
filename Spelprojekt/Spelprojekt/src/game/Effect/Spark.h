#ifndef SPARK_H
#define SPARK_H

#include "BaseEffect.h"

class Spark : public BaseEffect
{

public:

	Spark();
	~Spark();

	void copy(BaseEffect* s);

	void init(float x, float y, float z);
	void setSpawn(float x, float y, float z);
	void update();

	void fade();

	bool isFading();
	Light* getLights(int &nrLights)
	{
		nrLights = currentLights;
		return lights;
	}
	void setIntensity(int soIntense)
	{
		currentLights = soIntense;
	}
private:
	int currentLights;

};

#endif