#pragma once
#include "DamageSource.h"
#include "math/Vec3d.h"


#include <optional>

class PhaseList;
class EntityEnderCrystal;
class MultiPartEntityPart;

class IPhase {
public:
    virtual ~IPhase() = default;
    virtual bool getIsStationary() = 0;

    virtual void doClientRenderEffects() = 0;

    virtual void doLocalUpdate() = 0;

    virtual void onCrystalDestroyed(EntityEnderCrystal* var1, BlockPos var2, DamageSource::DamageSource var3, EntityPlayer* var4) = 0;

    virtual void initPhase() = 0;

    virtual void removeAreaEffect() = 0;

    virtual float getMaxRiseOrFall() = 0;

    virtual float getYawFactor() = 0;

    virtual PhaseList* getType() = 0;

    virtual std::optional<Vec3d> getTargetLocation() = 0;

    virtual float getAdjustedDamage(MultiPartEntityPart* var1, DamageSource::DamageSource var2, float var3) = 0;
};
