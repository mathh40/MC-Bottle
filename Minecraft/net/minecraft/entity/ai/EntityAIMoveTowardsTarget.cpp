#include "EntityAIMoveTowardsTarget.h"

#include "RandomPositionGenerator.h"
#include "math/Vec3d.h"

EntityAIMoveTowardsTarget::EntityAIMoveTowardsTarget(EntityCreature *creature, double speedIn, float targetMaxDistance)
    : creature(creature), speed(speedIn), maxTargetDistance(targetMaxDistance)
{
    setMutexBits(1);
}

bool EntityAIMoveTowardsTarget::shouldExecute()
{
    targetEntity = creature->getAttackTarget();
    if (targetEntity == nullptr)
    {
        return false;
    }
    else if (targetEntity->getDistanceSq(creature) > (double)(maxTargetDistance * maxTargetDistance))
    {
        return false;
    }
    else
    {
        auto vec3d = RandomPositionGenerator::findRandomTargetBlockTowards(
            creature, 16, 7, Vec3d(targetEntity->posX, targetEntity->posY, targetEntity->posZ));
        if (!vec3d)
        {
            return false;
        }
        else
        {
            movePosX = vec3d->getx();
            movePosY = vec3d->gety();
            movePosZ = vec3d->getz();
            return true;
        }
    }
}

bool EntityAIMoveTowardsTarget::shouldContinueExecuting()
{
    return !creature->getNavigator().noPath() && targetEntity->isEntityAlive() &&
           targetEntity->getDistanceSq(creature) < (double)(maxTargetDistance * maxTargetDistance);
}

void EntityAIMoveTowardsTarget::resetTask()
{
    targetEntity = nullptr;
}

void EntityAIMoveTowardsTarget::startExecuting()
{
    creature->getNavigator().tryMoveToXYZ(movePosX, movePosY, movePosZ, speed);
}
