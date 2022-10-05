#include "EntityAIOwnerHurtByTarget.h"

EntityAIOwnerHurtByTarget::EntityAIOwnerHurtByTarget(EntityTameable *theDefendingTameableIn)
    : EntityAITarget(theDefendingTameableIn, false), tameable(theDefendingTameableIn)
{
    setMutexBits(1);
}

bool EntityAIOwnerHurtByTarget::shouldExecute()
{
    if (!tameable->isTamed())
    {
        return false;
    }
    else
    {
        EntityLivingBase *entitylivingbase = tameable->getOwner();
        if (entitylivingbase == nullptr)
        {
            return false;
        }
        else
        {
            attacker = entitylivingbase->getRevengeTarget();
            auto i   = entitylivingbase->getRevengeTimer();
            return i != timestamp && isSuitableTarget(attacker, false) &&
                   tameable->shouldAttackEntity(attacker, entitylivingbase);
        }
    }
}

void EntityAIOwnerHurtByTarget::startExecuting()
{
    taskOwner->setAttackTarget(attacker);
    EntityLivingBase *entitylivingbase = tameable->getOwner();
    if (entitylivingbase != nullptr)
    {
        timestamp = entitylivingbase->getRevengeTimer();
    }

    EntityAITarget::startExecuting();
}
