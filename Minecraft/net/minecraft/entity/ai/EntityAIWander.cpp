#include "EntityAIWander.h"

#include "RandomPositionGenerator.h"

EntityAIWander::EntityAIWander(EntityCreature *creatureIn, double speedIn) : EntityAIWander(creatureIn, speedIn, 120)
{
}

EntityAIWander::EntityAIWander(EntityCreature *creatureIn, double speedIn, int32_t chance)
    : entity(creatureIn), speed(speedIn), executionChance(chance)
{
    setMutexBits(1);
}

bool EntityAIWander::shouldExecute()
{
    if (!mustUpdate)
    {
        if (entity->getIdleTime() >= 100)
        {
            return false;
        }

        if (entity->getRNG().nextInt(executionChance) != 0)
        {
            return false;
        }
    }

    auto vec3d = getPosition();
    if (!vec3d)
    {
        return false;
    }
    else
    {
        x          = vec3d->getx();
        y          = vec3d->gety();
        z          = vec3d->getz();
        mustUpdate = false;
        return true;
    }
}

bool EntityAIWander::shouldContinueExecuting()
{
    return !entity->getNavigator().noPath();
}

void EntityAIWander::startExecuting()
{
    entity->getNavigator().tryMoveToXYZ(x, y, z, speed);
}

void EntityAIWander::makeUpdate()
{
    mustUpdate = true;
}

void EntityAIWander::setExecutionChance(int32_t newchance)
{
    executionChance = newchance;
}

std::optional<Vec3d> EntityAIWander::getPosition()
{
    return RandomPositionGenerator::findRandomTarget(entity, 10, 7);
}
