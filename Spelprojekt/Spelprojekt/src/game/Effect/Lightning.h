#ifndef LIGHTNING_H
#define LIGHTNING_H

#include "BaseEffect.h"

class Lightning : public BaseEffect
{
public:

	Lightning();
	~Lightning();

	void copy(BaseEffect* s);

	void init(float x, float y, float z);

	void update();

	void setRotation(float rot);

	void fade();

	bool isFading();

private:

	float interpolate(float s0, float s1, float t);

	int currState;

	bool direction;

	float states[11];
	float idleTime;
	float idleStart;

	struct State
	{
		int s1;
		int s2;
		float baseTime;
	};

	State flashState[14];

};

#endif