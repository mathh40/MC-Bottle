#pragma once
#include "IPhase.h"

class EntityDragon;

class PhaseBase : public IPhase
{
  public:
    PhaseBase(EntityDragon *dragonIn);
    bool getIsStationary() override;
    void doClientRenderEffects() override;
    void doLocalUpdate() override;
    void onCrystalDestroyed(EntityEnderCrystal *crystal, const BlockPos &pos, DamageSource::DamageSource dmgSrc,
                            EntityPlayer *plyr);
    void initPhase() override;
    void removeAreaEffect() override;
    float getMaxRiseOrFall() override;
    std::optional<Vec3d> getTargetLocation() override;
    float getAdjustedDamage(MultiPartEntityPart *pt, DamageSource::DamageSource src, float damage) override;
    float getYawFactor() override;

  protected:
    EntityDragon *dragon;
};
