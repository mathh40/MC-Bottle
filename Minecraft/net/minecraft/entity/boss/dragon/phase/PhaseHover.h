#pragma once
#include "PhaseBase.h"

class PhaseHover : public PhaseBase
{
  public:
    PhaseHover(EntityDragon *dragonIn);
    void doLocalUpdate() override;
    bool getIsStationary() override;
    void initPhase() override;
    float getMaxRiseOrFall() override;
    std::optional<Vec3d> getTargetLocation() override;
    PhaseList *getType() override;

  private:
    std::optional<Vec3d> targetLocation;
};
