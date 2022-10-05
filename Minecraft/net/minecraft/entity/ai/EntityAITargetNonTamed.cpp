#include "EntityAITargetNonTamed.h"

EntityAITargetNonTamed::EntityAITargetNonTamed(EntityTameable *entityIn, std::type_index classTarget, bool checkSight,
                                               std::function<bool()> targetSelector)
    : EntityAINearestAttackableTarget(entityIn, classTarget, 10, checkSight, false, targetSelector), tameable(entityIn)
{
}

bool EntityAITargetNonTamed::shouldExecute()
{
    return !tameable->isTamed() && EntityAINearestAttackableTarget::shouldExecute();
}
