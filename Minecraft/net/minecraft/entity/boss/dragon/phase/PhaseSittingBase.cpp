#include "PhaseSittingBase.h"
#include "../../../Entity.h"

PhaseSittingBase::PhaseSittingBase(EntityDragon *p_i46794_1_)
    :PhaseBase(p_i46794_1_){
}

bool PhaseSittingBase::getIsStationary() {
    return true;
}

float PhaseSittingBase::getAdjustedDamage(MultiPartEntityPart* pt, DamageSource::DamageSource src, float damage) {
    if (Util::instanceof<EntityArrow>(src.getImmediateSource())) {
         src.getImmediateSource()->setFire(1);
         return 0.0F;
      } else {
         return PhaseBase::getAdjustedDamage(pt, src, damage);
      }
}
