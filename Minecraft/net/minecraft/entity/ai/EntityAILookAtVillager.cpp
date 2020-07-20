#include "EntityAILookAtVillager.h"
#include "EntityIronGolem.h"

EntityAILookAtVillager::EntityAILookAtVillager(EntityIronGolem *ironGolemIn)
    :ironGolem(ironGolemIn)
{
    setMutexBits(3);
}

bool EntityAILookAtVillager::shouldExecute()
{
    if (!ironGolem->world.isDaytime()) 
    {
        return false;
    }
    else if (ironGolem->getRNG().nextInt(8000) != 0) 
    {
        return false;
    }
    else 
    {
        villager = ironGolem->world.findNearestEntityWithinAABB(EntityVillager.class, ironGolem->getEntityBoundingBox().grow(6.0, 2.0, 6.0), ironGolem);
        return villager != nullptr;
    }
}

bool EntityAILookAtVillager::shouldContinueExecuting()
{
    return lookTime > 0;
}

void EntityAILookAtVillager::startExecuting()
{
    lookTime = 400;
    ironGolem->setHoldingRose(true);
}

void EntityAILookAtVillager::resetTask()
{
    ironGolem->setHoldingRose(false);
    villager = nullptr;
}

void EntityAILookAtVillager::updateTask()
{
    ironGolem->getLookHelper().setLookPositionWithEntity(villager, 30.0F, 30.0F);
    --lookTime;
}
