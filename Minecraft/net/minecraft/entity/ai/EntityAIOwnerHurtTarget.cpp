#include "EntityAIOwnerHurtTarget.h"

EntityAIOwnerHurtTarget::EntityAIOwnerHurtTarget(EntityTameable *theEntityTameableIn)
    : EntityAITarget(theEntityTameableIn, false), tameable(theEntityTameableIn)
{
    setMutexBits(1);
}

bool EntityAIOwnerHurtTarget::shouldExecute()
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
            attacker = entitylivingbase->getLastAttackedEntity();
            auto i   = entitylivingbase->getLastAttackedEntityTime();
            return i != timestamp && isSuitableTarget(attacker, false) &&
                   tameable->shouldAttackEntity(attacker, entitylivingbase);
        }
    }
}

void EntityAIOwnerHurtTarget::startExecuting()
{
    taskOwner->setAttackTarget(attacker);
    EntityLivingBase *entitylivingbase = tameable->getOwner();
    if (entitylivingbase != nullptr)
    {
        timestamp = entitylivingbase->getLastAttackedEntityTime();
    }

    EntityAITarget::startExecuting();
}
