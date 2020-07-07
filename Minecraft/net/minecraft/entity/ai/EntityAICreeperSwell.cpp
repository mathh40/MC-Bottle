#include "EntityAICreeperSwell.h"

EntityAICreeperSwell::EntityAICreeperSwell(EntityCreeper *entitycreeperIn)
    :swellingCreeper(entitycreeperIn)
{
    setMutexBits(1);
}

bool EntityAICreeperSwell::shouldExecute()
{
    EntityLivingBase* entitylivingbase = swellingCreeper->getAttackTarget();
    return swellingCreeper->getCreeperState() > 0 || entitylivingbase != nullptr && swellingCreeper->getDistanceSq(entitylivingbase) < 9.0;
}

void EntityAICreeperSwell::startExecuting()
{
    swellingCreeper->getNavigator().clearPath();
    creeperAttackTarget = swellingCreeper->getAttackTarget();
}

void EntityAICreeperSwell::resetTask()
{
    creeperAttackTarget = nullptr;
}

void EntityAICreeperSwell::updateTask()
{
    if (creeperAttackTarget == nullptr) 
    {
        swellingCreeper->setCreeperState(-1);
    }
    else if (swellingCreeper->getDistanceSq(creeperAttackTarget) > 49.0) 
    {
        swellingCreeper->setCreeperState(-1);
    }
    else if (!swellingCreeper->getEntitySenses().canSee(creeperAttackTarget)) 
    {
        swellingCreeper->setCreeperState(-1);
    }
    else 
    {
        swellingCreeper->setCreeperState(1);
    }
}
