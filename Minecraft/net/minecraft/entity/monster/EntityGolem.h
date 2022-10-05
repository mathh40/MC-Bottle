#pragma once
#include "../EntityCreature.h"
#include "../passive/IAnimals.h"

class EntityGolem : public EntityCreature, public IAnimals
{
  public:
    EntityGolem(World *worldIn);
    void fall(float distance, float damageMultiplier) override;
    int32_t getTalkInterval() override;

  protected:
    SoundEvent getAmbientSound() override;
    SoundEvent getHurtSound(DamageSource::DamageSource damageSourceIn) override;
    SoundEvent getDeathSound() override;
    bool canDespawn() override;
};
