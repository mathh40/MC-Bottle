#include "EntityAILandOnOwnersShoulder.h"
#include "EntityLivingBase.h"

EntityAILandOnOwnersShoulder::EntityAILandOnOwnersShoulder(EntityShoulderRiding* entityIn)
    :entity(entityIn)
{
    
}

bool EntityAILandOnOwnersShoulder::shouldExecute()
{
    EntityLivingBase* entitylivingbase = entity->getOwner();
    bool flag = entitylivingbase != nullptr && !((EntityPlayer*)entitylivingbase)->isSpectator() && !((EntityPlayer*)entitylivingbase)->capabilities.isFlying && !entitylivingbase->isInWater();
    return !entity->isSitting() && flag && entity->canSitOnShoulder();
}

bool EntityAILandOnOwnersShoulder::isInterruptible()
{
    return !isSittingOnShoulder;
}

void EntityAILandOnOwnersShoulder::startExecuting()
{
    owner = (EntityPlayer*)entity->getOwner();
    isSittingOnShoulder = false;
}

void EntityAILandOnOwnersShoulder::updateTask()
{
    if (!isSittingOnShoulder && !entity->isSitting() && !entity->getLeashed() && entity->getEntityBoundingBox().intersects(owner->getEntityBoundingBox())) 
    {
        isSittingOnShoulder = entity->setEntityOnShoulder(owner);
    }
}
