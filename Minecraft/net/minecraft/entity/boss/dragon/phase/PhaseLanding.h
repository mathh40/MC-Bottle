#pragma once
#include "PhaseBase.h"

class PhaseLanding : public PhaseBase
{
  public:
    PhaseLanding(EntityDragon *dragonIn);
    void doClientRenderEffects() override;
    void doLocalUpdate() override;
    float getMaxRiseOrFall() override;
    float getYawFactor() override;
    void initPhase() override;
    std::optional<Vec3d> getTargetLocation() override;
    PhaseList *getType() override;

  private:
    std::optional<Vec3d> targetLocation;
};
