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

		Light* getLights(int &nrLights);
		void setIntensity(int soIntense)
		{
			currentLights = soIntense;
		}

	private:
	
		float initSize;

		struct color
		{
			float r, g, b;
		};

		color* flameColor;

		int currentLights;
};

#endif