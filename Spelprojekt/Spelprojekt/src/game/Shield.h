#ifndef SHIELD_H
#define SHIELD_H

#include "BaseEffect.h"

class Shield : public BaseEffect
{
public:

	Shield();
	~Shield();

	void copy(BaseEffect* s);

	void init(float x, float y, float z);
	
	void update();

	void setRotation(float rot);
	
	void fade();

	bool isFading();

private:

	float rotation;

};


#endif