#include "PhaseBase.h"

#include "../../EntityDragon.h"

PhaseBase::PhaseBase(EntityDragon *dragonIn) : dragon(dragonIn)
{
}

bool PhaseBase::getIsStationary()
{
    return false;
}

void PhaseBase::doClientRenderEffects()
{
}

void PhaseBase::doLocalUpdate()
{
}

void PhaseBase::onCrystalDestroyed(EntityEnderCrystal *crystal, const BlockPos &pos, DamageSource::DamageSource dmgSrc,
                                   EntityPlayer *plyr)
{
}

void PhaseBase::initPhase()
{
}

void PhaseBase::removeAreaEffect()
{
}

float PhaseBase::getMaxRiseOrFall()
{
    return 0.6F;
}

std::optional<Vec3d> PhaseBase::getTargetLocation()
{
    return std::nullopt;
}

float PhaseBase::getAdjustedDamage(MultiPartEntityPart *pt, DamageSource::DamageSource src, float damage)
{
    return damage;
}

float PhaseBase::getYawFactor()
{
    float f  = MathHelper::sqrt(dragon->motionX * dragon->motionX + dragon->motionZ * dragon->motionZ) + 1.0F;
    float f1 = MathHelper::min(f, 40.0F);
    return 0.7F / f1 / f;
}
