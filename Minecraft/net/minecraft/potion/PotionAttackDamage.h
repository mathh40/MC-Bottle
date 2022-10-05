#pragma once
#include "Potion.h"

class PotionAttackDamage : public Potion
{
  public:
    double getAttributeModifierAmount(int32_t amplifier, AttributeModifier modifier);

  protected:
    PotionAttackDamage(bool isBadEffectIn, int32_t liquidColorIn, double bonusPerLevelIn);

    double bonusPerLevel;

  private:
};
