#ifndef SPARK_H
#define SPARK_H

#include "BaseEffect.h"

class Spark : public BaseEffect
{

public:

	Spark();
	virtual ~Spark();

	virtual void copy(BaseEffect* s);

	virtual void init(float x, float y, float z);
	virtual void setSpawn(float x, float y, float z);
	virtual void update();

	virtual void fade();

	virtual bool isFading();
	virtual Light* getLights(int &nrLights)
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