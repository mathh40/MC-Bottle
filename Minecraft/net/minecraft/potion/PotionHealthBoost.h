#pragma once
#include "Potion.h"

class PotionHealthBoost : public Potion
{
  public:
    PotionHealthBoost(bool isBadEffectIn, int32_t liquidColorIn);
    void removeAttributesModifiersFromEntity(EntityLivingBase *entityLivingBaseIn, AbstractAttributeMap attributeMapIn,
                                             int32_t amplifier);
};
