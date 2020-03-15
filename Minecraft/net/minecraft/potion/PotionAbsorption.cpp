#include "PotionAbsorption.h"

PotionAbsorption::PotionAbsorption(bool isBadEffectIn, int32_t liquidColorIn)
  :Potion(isBadEffectIn, liquidColorIn)
{
    
}

void PotionAbsorption::removeAttributesModifiersFromEntity(EntityLivingBase* entityLivingBaseIn, AbstractAttributeMap attributeMapIn, int32_t amplifier)
{
  entityLivingBaseIn->setAbsorptionAmount(entityLivingBaseIn->getAbsorptionAmount() - (float)(4 * (amplifier + 1)));
  Potion::removeAttributesModifiersFromEntity(entityLivingBaseIn, attributeMapIn, amplifier);
}
void PotionAbsorption::applyAttributesModifiersToEntity(EntityLivingBase* entityLivingBaseIn, AbstractAttributeMap attributeMapIn, int32_t amplifier)
{
  entityLivingBaseIn->setAbsorptionAmount(entityLivingBaseIn->getAbsorptionAmount() + (float)(4 * (amplifier + 1)));
  Potion::applyAttributesModifiersToEntity(entityLivingBaseIn, attributeMapIn, amplifier);
}