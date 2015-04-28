#ifndef TORCH
#define TORCH

#include "BaseEffect.h"

class Torch : public BaseEffect
{
	public:
	
		Torch();
		~Torch();
	
		virtual void copy(BaseEffect* t);

		virtual void init(float x, float y, float z);
		virtual void update();
		
		virtual void setSpawn(float x, float y, float z);
		
		void fade();

		bool isFading();

	private:
	
};

#endif