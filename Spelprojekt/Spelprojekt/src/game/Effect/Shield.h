#ifndef SHIELD_H
#define SHIELD_H

#include "BaseEffect.h"

class Shield : public BaseEffect
{
public:

	Shield();
	virtual ~Shield();

	virtual void copy(BaseEffect* s);

	virtual void init(float x, float y, float z);
	
	virtual void update();

	virtual void setRotation(float rot);
	
	virtual void fade();

	virtual bool isFading();

private:

	float rotation;

};


#endif