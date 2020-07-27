#include "EntityAIWatchClosest.h"

#include "../../inventory/ContainerPlayer.h"

EntityAIWatchClosest::EntityAIWatchClosest(EntityLiving *entityIn, std::type_index watchTargetClass, float maxDistance)
    :EntityAIWatchClosest(entityIn,watchTargetClass,maxDistance,0.02F)
{
    
}

EntityAIWatchClosest::EntityAIWatchClosest(EntityLiving *entityIn, std::type_index watchTargetClass, float maxDistance,
    float chanceIn)
        :entity(entityIn),watchedClass(watchTargetClass),maxDistance(maxDistance),chance(chanceIn)
{
    setMutexBits(2);
}

bool EntityAIWatchClosest::shouldExecute()
{
    if (entity->getRNG().nextFloat() >= chance) 
    {
        return false;
    }
    else 
    {
        if (entity->getAttackTarget() != nullptr) 
        {
            closestEntity = entity->getAttackTarget();
        }

        if (watchedClass == typeid(EntityPlayer)) 
        {
            closestEntity = entity->world.getClosestPlayer(entity->posX, entity->posY, entity->posZ, (double)maxDistance, Predicates.and(EntitySelectors.NOT_SPECTATING, EntitySelectors.notRiding(entity)));
        }
        else 
        {
            closestEntity = entity->world.findNearestEntityWithinAABB(watchedClass, entity->getEntityBoundingBox().grow((double)maxDistance, 3.0, (double)maxDistance), entity);
        }

        return closestEntity != nullptr;
    }
}

bool EntityAIWatchClosest::shouldContinueExecuting()
{
    if (!closestEntity->isEntityAlive()) 
    {
        return false;
    }
    else if (entity->getDistanceSq(closestEntity) > (double)(maxDistance * maxDistance)) 
    {
        return false;
    }
    else 
    {
        return lookTime > 0;
    }
}

void EntityAIWatchClosest::startExecuting()
{
    lookTime = 40 + entity->getRNG().nextInt(40);
}

void EntityAIWatchClosest::resetTask()
{
    closestEntity = nullptr;
}

void EntityAIWatchClosest::updateTask()
{
    entity->getLookHelper().setLookPosition(closestEntity.posX, closestEntity.posY + (double)closestEntity.getEyeHeight(), closestEntity.posZ, (float)entity->getHorizontalFaceSpeed(), (float)entity->getVerticalFaceSpeed());
    --lookTime;
}
