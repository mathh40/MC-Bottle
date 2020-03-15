#include "Potion.h"


#include "../world/storage/IPlayerFileData.h"
#include "DamageSource.h"
#include "StringUtils.h"
#include "math/MathHelper.h"


Potion* Potion::getPotionById(int32_t potionID)
{
  return REGISTRY.getObjectById(potionID).value();
}

int32_t Potion::getIdFromPotion(Potion* potionIn)
{
  return REGISTRY.getIDForObject(potionIn);
}

Potion * Potion::getPotionFromResourceLocation(std::string_view location)
{
  return REGISTRY.getObject(ResourceLocation(location)).value();
}

void Potion::performEffect(EntityLivingBase *entityLivingBaseIn, int32_t amplifier)
{
  if (this == MobEffects::REGENERATION)
  {
    if (entityLivingBaseIn.getHealth() < entityLivingBaseIn.getMaxHealth())
    {
      entityLivingBaseIn.heal(1.0F);
    }
  }
  else if (this == MobEffects::POISON) 
  {
    if (entityLivingBaseIn.getHealth() > 1.0F) 
    {
      entityLivingBaseIn.attackEntityFrom(DamageSource::MAGIC, 1.0F);
    }
  }
  else if (this == MobEffects::WITHER) 
  {
    entityLivingBaseIn.attackEntityFrom(DamageSource::WITHER, 1.0F);
  }
  else if (this == MobEffects::HUNGER && entityLivingBaseIn instanceof EntityPlayer) 
  {
    ((EntityPlayer)entityLivingBaseIn).addExhaustion(0.005F * (float)(amplifier + 1));
  }
  else if (this == MobEffects::SATURATION && entityLivingBaseIn instanceof EntityPlayer) 
  {
    if (!entityLivingBaseIn.world.isRemote) 
    {
      ((EntityPlayer)entityLivingBaseIn).getFoodStats().addStats(amplifier + 1, 1.0F);
    }
  }
  else if ((this != MobEffects::INSTANT_HEALTH || entityLivingBaseIn.isEntityUndead()) && (this != MobEffects.INSTANT_DAMAGE || !entityLivingBaseIn.isEntityUndead())) {
    if (this == MobEffects::INSTANT_DAMAGE && !entityLivingBaseIn.isEntityUndead() || this == MobEffects.INSTANT_HEALTH && entityLivingBaseIn.isEntityUndead()) {
      entityLivingBaseIn.attackEntityFrom(DamageSource::MAGIC, (float)(6 << amplifier));
    }
  }
  else 
  {
    entityLivingBaseIn.heal((float)Math.max(4 << amplifier, 0));
  }  
}

void Potion::affectEntity(Entity *source, Entity *indirectSource, EntityLivingBase *entityLivingBaseIn, int32_t amplifier, double health)
{
  int32_t j;
  if (this == MobEffects::INSTANT_HEALTH && !entityLivingBaseIn.isEntityUndead() || this == MobEffects::INSTANT_DAMAGE && entityLivingBaseIn.isEntityUndead()) 
  {
    j = (int)(health * (double)(4 << amplifier) + 0.5);
    entityLivingBaseIn.heal((float)j);
  }
  else if (this == MobEffects:::INSTANT_DAMAGE && !entityLivingBaseIn.isEntityUndead() || this == MobEffects::INSTANT_HEALTH && entityLivingBaseIn.isEntityUndead()) 
  {
    j = (int)(health * (double)(6 << amplifier) + 0.5);
    if (source == nullptr) 
    {
      entityLivingBaseIn.attackEntityFrom(DamageSource::MAGIC, (float)j);
    }
    else 
    {
      entityLivingBaseIn.attackEntityFrom(DamageSource::causeIndirectMagicDamage(source, indirectSource), (float)j);
    }
  }
}

bool Potion::isReady(int32_t duration, int32_t amplifier)
{
  int32_t i;
  if (this == MobEffects::REGENERATION) 
  {
    i = 50 >> amplifier;
    if (i > 0) 
    {
      return duration % i == 0;
    }
    else 
    {
      return true;
    }
  }
  else if (this == MobEffects::POISON) 
  {
    i = 25 >> amplifier;
    if (i > 0) 
    {
      return duration % i == 0;
    }
    else 
    {
      return true;
    }
  }
  else if (this == MobEffects::WITHER) 
  {
    i = 40 >> amplifier;
    if (i > 0) 
    {
      return duration % i == 0;
    }
    else
    {
      return true;
    }
  }
  else 
  {
    return this == MobEffects::HUNGER;
  }
}

bool Potion::isInstant()
{
  return false;
}

Potion& Potion::setPotionName(std::string_view nameIn)
{
  name = nameIn;
  return *this;
}

std::string Potion::getName() const
{
  return name;
}

bool Potion::hasStatusIcon() const
{
  return statusIconIndex >= 0;
}

int32_t Potion::getStatusIconIndex() const
{
  return statusIconIndex;
}

bool Potion::isBadEffect() const
{
  return bisBadEffect;
}

int32_t Potion::getLiquidColor() const
{
  return liquidColor;
}

Potion::Potion(bool isBadEffectIn, int32_t liquidColorIn)
  :bisBadEffect(isBadEffectIn),liquidColor(liquidColorIn)
{
  if (isBadEffectIn) 
  {
    effectiveness = 0.5;
  }
  else 
  {
    effectiveness = 1.0;
  }
}

Potion& Potion::setIconIndex(int32_t p_76399_1_, int32_t p_76399_2_)
{
  statusIconIndex = p_76399_1_ + p_76399_2_ * 8;
  return *this;
}

Potion & Potion::setEffectiveness(double effectivenessIn)
{
  effectiveness = effectivenessIn;
  return *this;
}


std::string Potion::getPotionDurationString(const PotionEffect& effect, float durationFactor)
{
  if (effect.getIsPotionDurationMax()) 
  {
    return "**:**";
  }
  else 
  {
    int32_t i = MathHelper::floor((float)effect.getDuration() * durationFactor);
    return StringUtils::ticksToElapsedTime(i);
  }
}

Potion& Potion::registerPotionAttributeModifier(IAttribute* attribute, std::string_view uniqueId, double ammount, int32_t operation)
{
  AttributeModifier attributemodifier = AttributeModifier(xg::Guid(uniqueId), getName(), ammount, operation);
  attributeModifierMap.put(attribute, attributemodifier);
  return *this;
}

std::unordered_map<IAttribute *, AttributeModifier>& Potion::getAttributeModifierMap()
{
  return attributeModifierMap;
}

void Potion::removeAttributesModifiersFromEntity(EntityLivingBase* entityLivingBaseIn, AbstractAttributeMap attributeMapIn, int32_t amplifier)
{
  Iterator var4 = attributeModifierMap.entrySet().iterator();

      while(var4.hasNext()) {
         Entry entry = (Entry)var4.next();
         IAttributeInstance iattributeinstance = attributeMapIn.getAttributeInstance((IAttribute)entry.getKey());
         if (iattributeinstance != nullptr) 
         {
            iattributeinstance.removeModifier((AttributeModifier)entry.getValue());
         }
      }
}

void Potion::applyAttributesModifiersToEntity(EntityLivingBase* entityLivingBaseIn, AbstractAttributeMap attributeMapIn, int32_t amplifier)
{
  Iterator var4 = attributeModifierMap.entrySet().iterator();

      while(var4.hasNext()) 
      {
         Entry entry = (Entry)var4.next();
         IAttributeInstance iattributeinstance = attributeMapIn.getAttributeInstance((IAttribute)entry.getKey());
         if (iattributeinstance != nullptr) 
         {
            AttributeModifier attributemodifier = (AttributeModifier)entry.getValue();
            iattributeinstance.removeModifier(attributemodifier);
            iattributeinstance.applyModifier(new AttributeModifier(attributemodifier.getID(), this.getName() + " " + amplifier, this.getAttributeModifierAmount(amplifier, attributemodifier), attributemodifier.getOperation()));
         }
      }
}

double Potion::getAttributeModifierAmount(int32_t amplifier, AttributeModifier modifier)
{
  return modifier.getAmount() * (double)(amplifier + 1);
}

bool Potion::isBeneficial() const
{
  return beneficial;
}

Potion& Potion::setBeneficial()
{
  beneficial = true;
  return *this;
}

void Potion::registerPotions()
{
  REGISTRY.registe(1, ResourceLocation("speed"), (new Potion(false, 8171462)).setPotionName("effect.moveSpeed").setIconIndex(0, 0).registerPotionAttributeModifier(SharedMonsterAttributes.MOVEMENT_SPEED, "91AEAA56-376B-4498-935B-2F7F68070635", 0.20000000298023224D, 2).setBeneficial());
      REGISTRY.registe(2, ResourceLocation("slowness"), (new Potion(true, 5926017)).setPotionName("effect.moveSlowdown").setIconIndex(1, 0).registerPotionAttributeModifier(SharedMonsterAttributes.MOVEMENT_SPEED, "7107DE5E-7CE8-4030-940E-514C1F160890", -0.15000000596046448D, 2));
      REGISTRY.registe(3, ResourceLocation("haste"), (new Potion(false, 14270531)).setPotionName("effect.digSpeed").setIconIndex(2, 0).setEffectiveness(1.5D).setBeneficial().registerPotionAttributeModifier(SharedMonsterAttributes.ATTACK_SPEED, "AF8B6E3F-3328-4C0A-AA36-5BA2BB9DBEF3", 0.10000000149011612D, 2));
      REGISTRY.registe(4, ResourceLocation("strength"), (new PotionAttackDamage(false, 9643043, 3.0D)).setPotionName("effect.damageBoost").setIconIndex(4, 0).registerPotionAttributeModifier(SharedMonsterAttributes.ATTACK_DAMAGE, "648D7064-6A60-4F59-8ABE-C2C23A6DD7A9", 0.0D, 0).setBeneficial());
      REGISTRY.registe(6, ResourceLocation("instant_health"), (new PotionHealth(false, 16262179)).setPotionName("effect.heal").setBeneficial());
      REGISTRY.registe(7, ResourceLocation("instant_damage"), (new PotionHealth(true, 4393481)).setPotionName("effect.harm").setBeneficial());
      REGISTRY.registe(8, ResourceLocation("jump_boost"), (new Potion(false, 2293580)).setPotionName("effect.jump").setIconIndex(2, 1).setBeneficial());
      REGISTRY.registe(9, ResourceLocation("nausea"), (new Potion(true, 5578058)).setPotionName("effect.confusion").setIconIndex(3, 1).setEffectiveness(0.25D));
      REGISTRY.registe(10, ResourceLocation("regeneration"), (new Potion(false, 13458603)).setPotionName("effect.regeneration").setIconIndex(7, 0).setEffectiveness(0.25D).setBeneficial());
      REGISTRY.registe(11, ResourceLocation("resistance"), (new Potion(false, 10044730)).setPotionName("effect.resistance").setIconIndex(6, 1).setBeneficial());
      REGISTRY.registe(12, ResourceLocation("fire_resistance"), (new Potion(false, 14981690)).setPotionName("effect.fireResistance").setIconIndex(7, 1).setBeneficial());
      REGISTRY.registe(13, ResourceLocation("water_breathing"), (new Potion(false, 3035801)).setPotionName("effect.waterBreathing").setIconIndex(0, 2).setBeneficial());
      REGISTRY.registe(14, ResourceLocation("invisibility"), (new Potion(false, 8356754)).setPotionName("effect.invisibility").setIconIndex(0, 1).setBeneficial());
      REGISTRY.registe(15, ResourceLocation("blindness"), (new Potion(true, 2039587)).setPotionName("effect.blindness").setIconIndex(5, 1).setEffectiveness(0.25D));
      REGISTRY.registe(16, ResourceLocation("night_vision"), (new Potion(false, 2039713)).setPotionName("effect.nightVision").setIconIndex(4, 1).setBeneficial());
      REGISTRY.registe(17, ResourceLocation("hunger"), (new Potion(true, 5797459)).setPotionName("effect.hunger").setIconIndex(1, 1));
      REGISTRY.registe(18, ResourceLocation("weakness"), (new PotionAttackDamage(true, 4738376, -4.0D)).setPotionName("effect.weakness").setIconIndex(5, 0).registerPotionAttributeModifier(SharedMonsterAttributes.ATTACK_DAMAGE, "22653B89-116E-49DC-9B6B-9971489B5BE5", 0.0D, 0));
      REGISTRY.registe(19, ResourceLocation("poison"), (new Potion(true, 5149489)).setPotionName("effect.poison").setIconIndex(6, 0).setEffectiveness(0.25D));
      REGISTRY.registe(20, ResourceLocation("wither"), (new Potion(true, 3484199)).setPotionName("effect.wither").setIconIndex(1, 2).setEffectiveness(0.25D));
      REGISTRY.registe(21, ResourceLocation("health_boost"), (new PotionHealthBoost(false, 16284963)).setPotionName("effect.healthBoost").setIconIndex(7, 2).registerPotionAttributeModifier(SharedMonsterAttributes.MAX_HEALTH, "5D6F0BA2-1186-46AC-B896-C61C5CEE99CC", 4.0D, 0).setBeneficial());
      REGISTRY.registe(22, ResourceLocation("absorption"), (new PotionAbsorption(false, 2445989)).setPotionName("effect.absorption").setIconIndex(2, 2).setBeneficial());
      REGISTRY.registe(23, ResourceLocation("saturation"), (new PotionHealth(false, 16262179)).setPotionName("effect.saturation").setBeneficial());
      REGISTRY.registe(24, ResourceLocation("glowing"), (new Potion(false, 9740385)).setPotionName("effect.glowing").setIconIndex(4, 2));
      REGISTRY.registe(25, ResourceLocation("levitation"), (new Potion(true, 13565951)).setPotionName("effect.levitation").setIconIndex(3, 2));
      REGISTRY.registe(26, ResourceLocation("luck"), (new Potion(false, 3381504)).setPotionName("effect.luck").setIconIndex(5, 2).setBeneficial().registerPotionAttributeModifier(SharedMonsterAttributes.LUCK, "03C3C89D-7037-4B42-869F-B146BCB64D2E", 1.0D, 0));
      REGISTRY.registe(27, ResourceLocation("unluck"), (new Potion(true, 12624973)).setPotionName("effect.unluck").setIconIndex(6, 2).registerPotionAttributeModifier(SharedMonsterAttributes.LUCK, "CC5AF142-2BD2-4215-B636-2605AED11727", -1.0D, 0));
}
