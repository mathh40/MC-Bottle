#include "EntityAIPlay.h"


#include "RandomPositionGenerator.h"
#include "../../../../../spdlog/include/spdlog/fmt/bundled/format.h"
#include "math/Vec3d.h"

EntityAIPlay::EntityAIPlay(EntityVillager *villagerIn, double speedIn)
    :villager(villagerIn),speed(speedIn)
{
    setMutexBits(1);
}

bool EntityAIPlay::shouldExecute()
{
    if (villager->getGrowingAge() >= 0) 
    {
        return false;
    }
    else if (villager->getRNG().nextInt(400) != 0) 
    {
        return false;
    }
    else 
    {
        auto list = villager->world->getEntitiesWithinAABB(EntityVillager.class, villager->getEntityBoundingBox().grow(6.0, 3.0, 6.0));
        double d0 = Double.MAX_VALUE;

        for(auto entityvillager : list)
        {
            if (entityvillager != villager && !entityvillager->isPlaying() && entityvillager->getGrowingAge() < 0) 
            {
                double d1 = entityvillager->getDistanceSq(villager);
                if (d1 <= d0) {
                    d0 = d1;
                    targetVillager = entityvillager;
                }
            }
        }

        if (targetVillager == nullptr) 
        {
            auto vec3d = RandomPositionGenerator::findRandomTarget(villager, 16, 3);
            if (!vec3d) 
            {
                return false;
            }
        }

        return true;
    }
}

bool EntityAIPlay::shouldContinueExecuting()
{
    return playTime > 0;
}

void EntityAIPlay::startExecuting()
{
    if (targetVillager != nullptr) 
    {
        villager->setPlaying(true);
    }

    playTime = 1000;
}

void EntityAIPlay::resetTask()
{
    villager->setPlaying(false);
    targetVillager = nullptr;
}

void EntityAIPlay::updateTask()
{
    --playTime;
    if (targetVillager != nullptr) 
    {
        if (villager->getDistanceSq(targetVillager) > 4.0) 
        {
            villager->getNavigator().tryMoveToEntityLiving(targetVillager, speed);
        }
    }
    else if (villager->getNavigator().noPath()) 
    {
        auto vec3d = RandomPositionGenerator::findRandomTarget(villager, 16, 3);
        if (!vec3d) 
        {
            return;
        }

        villager->getNavigator().tryMoveToXYZ(vec3d->getx(), vec3d->gety(), vec3d->getz(), speed);
    }
}
