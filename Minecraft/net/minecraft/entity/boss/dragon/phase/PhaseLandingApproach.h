#pragma once
#include "../../../../pathfinding/Path.h"
#include "PhaseBase.h"

class PhaseLandingApproach : public PhaseBase
{
  public:
    PhaseLandingApproach(EntityDragon *dragonIn);
    PhaseList *getType() override;
    void initPhase() override;
    void doLocalUpdate() override;
    std::optional<Vec3d> getTargetLocation() override;

  private:
    void findNewTarget();
    void navigateToNextPathNode();

    std::optional<Path> currentPath;
    std::optional<Vec3d> targetLocation;
};
