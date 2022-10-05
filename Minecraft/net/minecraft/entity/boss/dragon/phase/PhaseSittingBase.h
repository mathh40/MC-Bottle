#pragma once
#include "PhaseBase.h"

class PhaseSittingBase : public PhaseBase
{
  public:
    PhaseSittingBase(EntityDragon *p_i46794_1_);
    bool getIsStationary() override;
    float getAdjustedDamage(MultiPartEntityPart *pt, DamageSource::DamageSource src, float damage) override;

  private:
};
