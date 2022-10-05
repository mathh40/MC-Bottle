#include "EntityGolem.h"

EntityGolem::EntityGolem(World *worldIn) : EntityCreature(worldIn)
{
}

void EntityGolem::fall(float distance, float damageMultiplier)
{
}

int32_t EntityGolem::getTalkInterval()
{
    return 120;
}

SoundEvent EntityGolem::getAmbientSound()
{
    return std::nullopt;
    ;
}

SoundEvent EntityGolem::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return std::nullopt;
}

SoundEvent EntityGolem::getDeathSound()
{
    return std::nullopt;
}

bool EntityGolem::canDespawn()
{
    return false;
}
