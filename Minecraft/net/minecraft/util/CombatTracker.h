#pragma once
#include <string>
#include <vector>
#include "CombatEntry.h"

class EntityLivingBase;

class CombatTracker
{
public:
	CombatTracker(EntityLivingBase* fighterIn);
	void calculateFallSuffix();
	void trackDamage(const DamageSource::DamageSource &damageSrc, const float healthIn, const float damageAmount);
	ITextComponent* getDeathMessage();
	EntityLivingBase* getBestAttacker();
	
	int32_t getCombatDuration() const;
	void reset();
	EntityLivingBase* getFighter() const;

private:
	std::vector< std::shared_ptr<CombatEntry>> combatEntries;
	EntityLivingBase* fighter;
	int32_t lastDamageTime;
	int32_t combatStartTime;
	int32_t combatEndTime;
	bool inCombat;
	bool takingDamage;
	std::string fallSuffix;

	CombatEntry getBestCombatEntry();
    static std::string getFallSuffix(CombatEntry entry);
	void resetFallSuffix();
};
