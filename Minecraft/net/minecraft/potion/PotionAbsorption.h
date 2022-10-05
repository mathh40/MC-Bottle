#pragma once
#include "Potion.h"

class PotionAbsorption : public Potion
{
  public:
    void removeAttributesModifiersFromEntity(EntityLivingBase *entityLivingBaseIn, AbstractAttributeMap attributeMapIn,
                                             int32_t amplifier);
    void applyAttributesModifiersToEntity(EntityLivingBase *entityLivingBaseIn, AbstractAttributeMap attributeMapIn,
                                          int32_t amplifier);

  protected:
    PotionAbsorption(bool isBadEffectIn, int32_t liquidColorIn);

  private:
};
