#include "PotionHealthBoost.h"

PotionHealthBoost::PotionHealthBoost(bool isBadEffectIn, int32_t liquidColorIn)
  :Potion(isBadEffectIn, liquidColorIn)
{    
}

void PotionHealthBoost::removeAttributesModifiersFromEntity(EntityLivingBase* entityLivingBaseIn, AbstractAttributeMap attributeMapIn, int32_t amplifier)
{
  Potion::removeAttributesModifiersFromEntity(entityLivingBaseIn, attributeMapIn, amplifier);
  if (entityLivingBaseIn->getHealth() > entityLivingBaseIn->getMaxHealth()) 
  {
    entityLivingBaseIn->setHealth(entityLivingBaseIn->getMaxHealth());
  }
}