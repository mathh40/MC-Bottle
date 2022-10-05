#include "EntityAISit.h"

class EntityLivingBase;

EntityAISit::EntityAISit(EntityTameable *entityIn) : tameable(entityIn)
{
    setMutexBits(5);
}

bool EntityAISit::shouldExecute()
{
    if (!tameable->isTamed())
    {
        return false;
    }
    else if (tameable->isInWater())
    {
        return false;
    }
    else if (!tameable->onGround)
    {
        return false;
    }
    else
    {
        EntityLivingBase *entitylivingbase = tameable->getOwner();
        if (entitylivingbase == nullptr)
        {
            return true;
        }
        else
        {
            return tameable->getDistanceSq(entitylivingbase) < 144.0 && entitylivingbase->getRevengeTarget() != nullptr
                       ? false
                       : isSitting;
        }
    }
}

void EntityAISit::startExecuting()
{
    tameable->getNavigator().clearPath();
    tameable->setSitting(true);
}

void EntityAISit::resetTask()
{
    tameable->setSitting(false);
}

void EntityAISit::setSitting(bool sitting)
{
    isSitting = sitting;
}
