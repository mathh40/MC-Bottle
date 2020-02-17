#pragma once
#include <string>
#include "instanceof.h"

class CombatEntry
{
public:

	CombatEntry(const DamageSource &damageSrcIn, const int timeIn, const float healthAmount, const float damageAmount, const std::string &fallSuffixIn, const float fallDistanceIn);
	DamageSource getDamageSrc();
	float getDamage();
	bool isLivingDamageSrc();
	std::string getFallSuffix();
	ITextComponent getDamageSrcDisplayName();
	float getDamageAmount();
private:
	DamageSource damageSrc;
	int time;
	float damage;
	float health;
	std::string fallSuffix;
	float fallDistance;
};