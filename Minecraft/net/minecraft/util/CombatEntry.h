#pragma once
#include <string>

#include "text/ITextComponent.h"

namespace DamageSource {
	class DamageSource;
}

class CombatEntry
{
public:
	CombatEntry(const DamageSource::DamageSource &damageSrcIn, uint64_t timeIn, float healthAmount,
               float damageAmount, const std::string &fallSuffixIn, float fallDistanceIn);
	DamageSource::DamageSource getDamageSrc();
	float getDamage();
	bool isLivingDamageSrc();
	std::string getFallSuffix();
	ITextComponent* getDamageSrcDisplayName();
	float getDamageAmount();
private:
	DamageSource::DamageSource damageSrc;
	uint64_t time;
	float damage;
	float health;
	std::string fallSuffix;
	float fallDistance;
};
