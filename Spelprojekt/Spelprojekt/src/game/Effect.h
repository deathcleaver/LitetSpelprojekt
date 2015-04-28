#ifndef EFFECT_H
#define EFFECT_H

#include "../legend.h"
#include "Torch.h"
#include "Spark.h"

class Effect
{
public:
	Effect();
	~Effect();

	void create(EffectType effType);
	void reCreate(EffectType newType);
	
	EffectType getType();

	Torch* getTorchEffect();
	Spark* getSparkEffect();

	void update();

private:

	void clear();

	EffectType effectType = EffectType::NONE;

	Torch* torchEffect;
	Spark* sparkEffect;

};

#endif