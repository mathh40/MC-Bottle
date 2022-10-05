#pragma once
#include "PhaseSittingBase.h"

class PhaseSittingAttacking : public PhaseSittingBase
{
  public:
    PhaseSittingAttacking(EntityDragon *dragonIn);
    void doClientRenderEffects() override;
    void doLocalUpdate() override;
    void initPhase() override;
    PhaseList *getType() override;

  private:
    int32_t attackingTicks;
};
