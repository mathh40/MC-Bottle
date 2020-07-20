#include "EntityAIOcelotAttack.h"

#include "math/MathHelper.h"

EntityAIOcelotAttack::EntityAIOcelotAttack(EntityLiving *theEntityIn)
    :entity(theEntityIn),world(theEntityIn->world)
{
    setMutexBits(3);
}

bool EntityAIOcelotAttack::shouldExecute()
{
    EntityLivingBase* entitylivingbase = entity->getAttackTarget();
    if (entitylivingbase == nullptr) 
    {
        return false;
    }
    else 
    {
        target = entitylivingbase;
        return true;
    }
}

bool EntityAIOcelotAttack::shouldContinueExecuting()
{
    if (!target->isEntityAlive()) 
    {
        return false;
    }
    else if (entity->getDistanceSq(target) > 225.0) 
    {
        return false;
    }
    else 
    {
        return !entity->getNavigator().noPath() || shouldExecute();
    }
}

void EntityAIOcelotAttack::resetTask()
{
    target = nullptr;
    entity->getNavigator().clearPath();
}

void EntityAIOcelotAttack::updateTask()
{
    entity->getLookHelper().setLookPositionWithEntity(target, 30.0F, 30.0F);
    double d0 = (double)(entity->width * 2.0F * entity->width * 2.0F);
    double d1 = entity->getDistanceSq(target->posX, target->getEntityBoundingBox().minY, target->posZ);
    double d2 = 0.8;
    if (d1 > d0 && d1 < 16.0) 
    {
        d2 = 1.33;
    }
    else if (d1 < 225.0) 
    {
        d2 = 0.6;
    }

    entity->getNavigator().tryMoveToEntityLiving(target, d2);
    attackCountdown = MathHelper::max(attackCountdown - 1, 0);
    if (d1 <= d0 && attackCountdown <= 0) 
    {
        attackCountdown = 20;
        entity->attackEntityAsMob(target);
    }
}
