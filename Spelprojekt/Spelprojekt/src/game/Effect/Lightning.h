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

	bool direction;

};

#endif