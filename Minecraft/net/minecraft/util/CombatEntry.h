#pragma once
#include <string>

#include "text/ITextComponent.h"

namespace DamageSource {
	class DamageSource;
}

class CombatEntry
{
public:

	CombatEntry(const DamageSource::DamageSource &damageSrcIn, const int timeIn, const float healthAmount, const float damageAmount, const std::string &fallSuffixIn, const float fallDistanceIn);
	DamageSource::DamageSource getDamageSrc();
	float getDamage();
	bool isLivingDamageSrc();
	std::string getFallSuffix();
	ITextComponent* getDamageSrcDisplayName();
	float getDamageAmount();
private:
	DamageSource::DamageSource damageSrc;
	int time;
	float damage;
	float health;
	std::string fallSuffix;
	float fallDistance;
};
