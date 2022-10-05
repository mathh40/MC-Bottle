#pragma once
#include "../../../../pathfinding/Path.h"
#include "PhaseBase.h"

class PhaseStrafePlayer : public PhaseBase
{
  public:
    PhaseStrafePlayer(EntityDragon *dragonIn);
    void doLocalUpdate() override;
    void initPhase() override;
    void setTarget(EntityLivingBase *p_188686_1_);
    std::optional<Vec3d> getTargetLocation() override;
    PhaseList *getType() override;

  private:
    void findNewTarget();
    void navigateToNextPathNode();

    static std::shared_ptr<spdlog::logger> PhaseStrafePlayer::LOGGER;
    int fireballCharge;
    std::optional<Path> currentPath;
    std::optional<Vec3d> targetLocation;
    EntityLivingBase *attackTarget;
    bool holdingPatternClockwise;
};
