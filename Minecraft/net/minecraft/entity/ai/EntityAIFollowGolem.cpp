#include "EntityAIFollowGolem.h"

#include <vector>

EntityAIFollowGolem::EntityAIFollowGolem(EntityVillager *villagerIn)
    :villager(villagerIn)
{
    setMutexBits(3);
}

bool EntityAIFollowGolem::shouldExecute()
{
    if (villager->getGrowingAge() >= 0) 
    {
        return false;
    }
    else if (!villager->world.isDaytime()) 
    {
        return false;
    }
    else 
    {
        std::vector<EntityIronGolem*> list = villager->world.getEntitiesWithinAABB(EntityIronGolem.class, villager->getEntityBoundingBox().grow(6.0, 2.0, 6.0));
        if (list.empty()) 
        {
            return false;
        }
        else 
        {
            for(auto& entityirongolem :list)
            {
                if (entityirongolem.getHoldRoseTick() > 0) 
                {
                    ironGolem = entityirongolem;
                    break;
                }
            }

            return ironGolem != nullptr;
        }
    }
}

bool EntityAIFollowGolem::shouldContinueExecuting()
{
    return ironGolem->getHoldRoseTick() > 0;
}

void EntityAIFollowGolem::startExecuting()
{
    takeGolemRoseTick = villager->getRNG()(320);
    tookGolemRose = false;
    ironGolem->getNavigator().clearPath();
}

void EntityAIFollowGolem::resetTask()
{
    ironGolem = nullptr;
    villager->getNavigator().clearPath();
}

void EntityAIFollowGolem::updateTask()
{
    villager->getLookHelper().setLookPositionWithEntity(ironGolem, 30.0F, 30.0F);
    if (ironGolem->getHoldRoseTick() == takeGolemRoseTick) 
    {
        villager->getNavigator().tryMoveToEntityLiving(ironGolem, 0.5);
        tookGolemRose = true;
    }

    if (tookGolemRose && villager->getDistanceSq(ironGolem) < 4.0) 
    {
        ironGolem->setHoldingRose(false);
        villager->getNavigator().clearPath();
    }
}
