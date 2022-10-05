#include "EntityAIRunAroundLikeCrazy.h"

#include "../Entity.h"
#include "RandomPositionGenerator.h"

EntityAIRunAroundLikeCrazy::EntityAIRunAroundLikeCrazy(AbstractHorse *horse, double speedIn)
    : horseHost(horse), speed(speedIn)
{
    setMutexBits(1);
}

bool EntityAIRunAroundLikeCrazy::shouldExecute()
{
    if (!horseHost->isTame() && horseHost->isBeingRidden())
    {
        auto vec3d = RandomPositionGenerator::findRandomTarget(horseHost, 5, 4);
        if (!vec3d)
        {
            return false;
        }
        else
        {
            targetX = vec3d->getx();
            targetY = vec3d->gety();
            targetZ = vec3d->getz();
            return true;
        }
    }
    else
    {
        return false;
    }
}

void EntityAIRunAroundLikeCrazy::startExecuting()
{
    horseHost->getNavigator().tryMoveToXYZ(targetX, targetY, targetZ, speed);
}

bool EntityAIRunAroundLikeCrazy::shouldContinueExecuting()
{
    return !horseHost->isTame() && !horseHost->getNavigator().noPath() && horseHost->isBeingRidden();
}

void EntityAIRunAroundLikeCrazy::updateTask()
{
    if (!horseHost->isTame() && horseHost->getRNG().nextInt(50) == 0)
    {
        Entity *entity = (Entity *)horseHost->getPassengers().get(0);
        if (entity == nullptr)
        {
            return;
        }

        if (Util:: instanceof <EntityPlayer>(entity))
        {
            auto i = horseHost->getTemper();
            auto j = horseHost->getMaxTemper();
            if (j > 0 && horseHost->getRNG().nextInt(j) < i)
            {
                horseHost->setTamedBy((EntityPlayer *)entity);
                return;
            }

            horseHost->increaseTemper(5);
        }

        horseHost->removePassengers();
        horseHost->makeMad();
        horseHost->world.setEntityState(horseHost, std::byte{6});
    }
}
