#include "Effect.h"

Effect::Effect()
{
	torchEffect = 0;
	sparkEffect = 0;
}

Effect::~Effect()
{
	clear();
}


void Effect::clear()
{
	if (torchEffect)
		delete torchEffect;
	if (sparkEffect)
		delete sparkEffect;

	torchEffect = 0;
	sparkEffect = 0;
}

void Effect::create(EffectType effType)
{
	effectType = effType;
	switch (effectType)
	{
	case EffectType::torch:
	{
		torchEffect = new Torch();
		break;
	}
	case EffectType::spark:
	{
		sparkEffect = new Spark();
		break;
	}
	default:
		break;
	}
}

void Effect::reCreate(EffectType effType)
{
	clear();
	create(effType);
}

EffectType Effect::getType()
{
	return effectType;

}

Torch* Effect::getTorchEffect()
{
	return torchEffect;
}

Spark* Effect::getSparkEffect()
{
	return sparkEffect;
}

void Effect::update()
{
	if (torchEffect)
		torchEffect->update();
	if (sparkEffect)
		sparkEffect->update();
}