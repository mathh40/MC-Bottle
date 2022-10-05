#include "PotionHealth.h"

PotionHealth::PotionHealth(bool isBadEffectIn, int32_t liquidColorIn) : Potion(isBadEffectIn, liquidColorIn)
{
}

bool PotionHealth::isInstant() const
{
    return true;
}

bool PotionHealth::isReady(int32_t duration, int32_t amplifier) const
{
    return duration >= 1;
}
