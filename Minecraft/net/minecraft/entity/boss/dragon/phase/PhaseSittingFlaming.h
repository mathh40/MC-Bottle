#pragma once
#include "PhaseSittingBase.h"

class EntityAreaEffectCloud;

class PhaseSittingFlaming : public PhaseSittingBase
{
  public:
    PhaseSittingFlaming(EntityDragon *dragonIn);
    void doClientRenderEffects() override;
    void doLocalUpdate() override;
    void initPhase() override;
    void removeAreaEffect() override;
    PhaseList *getType() override;
    void resetFlameCount();

  private:
    int32_t flameTicks;
    int32_t flameCount;
    EntityAreaEffectCloud *areaEffectCloud;
};
