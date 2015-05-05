#include "Effect.h"

Effect::Effect()
{
	effect = 0;
}

Effect::~Effect()
{
	clear();
}


void Effect::clear()
{
	if (effect)
		delete effect;

	effect = 0;
}

void Effect::create(EffectType effType)
{
	effectType = effType;
	switch (effectType)
	{
	case EffectType::torch:
	{
		effect = new Torch();
		break;
	}
	case EffectType::spark:
	{
		effect = new Spark();
		break;
	}
	case EffectType::shield:
	{
		effect = new Shield();
		break;
	}
	case EffectType::lightning:
	{
		effect = new Lightning();
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

BaseEffect* Effect::getEffect()
{
	return effect;
}


void Effect::update()
{
	effect->update();
}