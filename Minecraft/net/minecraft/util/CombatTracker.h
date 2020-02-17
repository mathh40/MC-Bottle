#pragma once
#include <string>
#include <vector>
#include "CombatEntry.h"

class CombatTracker
{
public:
	CombatTracker(const EntityLivingBase &fighterIn);
	void calculateFallSuffix();
	void trackDamage(const DamageSource &damageSrc, const float healthIn, const float damageAmount);
	ITextComponent getDeathMessage();
	EntityLivingBase getBestAttacker();
	
	int getCombatDuration();
	void reset();
	EntityLivingBase getFighter();

private:
	std::vector< std::shared_ptr<CombatEntry>> combatEntries;
	EntityLivingBase fighter;
	int lastDamageTime;
	int combatStartTime;
	int combatEndTime;
	bool inCombat;
	bool takingDamage;
	std::string fallSuffix;

	CombatEntry getBestCombatEntry();
	std::string getFallSuffix(CombatEntry entry);
	void resetFallSuffix();
};