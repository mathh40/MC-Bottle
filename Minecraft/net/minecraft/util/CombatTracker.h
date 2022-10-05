#pragma once
#include "CombatEntry.h"

#include <string>
#include <vector>

class EntityLivingBase;

class CombatTracker
{
  public:
    CombatTracker(EntityLivingBase *fighterIn);
    void calculateFallSuffix();
    void trackDamage(const DamageSource::DamageSource &damageSrc, const float healthIn, const float damageAmount);
    std::shared_ptr<ITextComponent> getDeathMessage();
    EntityLivingBase *getBestAttacker();

    uint32_t getCombatDuration() const;
    void reset();
    EntityLivingBase *getFighter() const;

  private:
    std::vector<std::unique_ptr<CombatEntry>> combatEntries;
    EntityLivingBase *fighter;
    uint32_t lastDamageTime;
    uint32_t combatStartTime;
    uint32_t combatEndTime;
    bool inCombat;
    bool takingDamage;
    std::string fallSuffix;

    CombatEntry *getBestCombatEntry();
    static std::string getFallSuffix(CombatEntry *entry);
    void resetFallSuffix();
};
