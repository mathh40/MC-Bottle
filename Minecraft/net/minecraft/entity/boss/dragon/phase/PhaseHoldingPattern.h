#pragma once
#include "../../../../pathfinding/Path.h"
#include "PhaseBase.h"

class PhaseHoldingPattern : public PhaseBase
{
  public:
    PhaseHoldingPattern(EntityDragon *dragonIn);
    PhaseList *getType() override;
    void doLocalUpdate() override;
    void initPhase() override;
    std::optional<Vec3d> getTargetLocation() override;

  private:
    void findNewTarget();
    void strafePlayer(EntityPlayer *player);

    std::optional<Path> currentPath;
    std::optional<Vec3d> targetLocation;
    bool clockwise;
};
