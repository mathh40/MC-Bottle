#include "PotionAttackDamage.h"

PotionAttackDamage::PotionAttackDamage(bool isBadEffectIn, int32_t liquidColorIn, double bonusPerLevelIn)
    : Potion(isBadEffectIn, liquidColorIn), bonusPerLevel(bonusPerLevelIn)
{
}

double Potion::getAttributeModifierAmount(int32_t amplifier, AttributeModifier modifier)
{
    return bonusPerLevel * (double)(amplifier + 1);
}
