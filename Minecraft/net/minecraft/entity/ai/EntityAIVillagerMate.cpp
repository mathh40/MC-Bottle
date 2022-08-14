#include "EntityAIVillagerMate.h"
#include "math/BlockPos.h"

EntityAIVillagerMate::EntityAIVillagerMate(EntityVillager *villagerIn)
    :villager(villagerIn),world(villagerIn->world)
{
    setMutexBits(3);
}

bool EntityAIVillagerMate::shouldExecute()
{
    if (villager->getGrowingAge() != 0) 
    {
        return false;
    }
    else if (villager->getRNG().nextInt(500) != 0) 
    {
        return false;
    }
    else 
    {
        village = world->getVillageCollection().getNearestVillage(BlockPos(villager), 0);
        if (village == nullptr) 
        {
            return false;
        }
        else if (checkSufficientDoorsPresentForNewVillager() && villager->getIsWillingToMate(true)) 
        {
            Entity* entity = world->findNearestEntityWithinAABB(typeid(EntityVillager), villager->getEntityBoundingBox().grow(8.0, 3.0, 8.0), villager);
            if (entity == nullptr) 
            {
                return false;
            }
            else 
            {
                mate = (EntityVillager*)entity;
                return mate->getGrowingAge() == 0 && mate->getIsWillingToMate(true);
            }
        }
        else 
        {
            return false;
        }
    }
}

void EntityAIVillagerMate::startExecuting()
{
    matingTimeout = 300;
    villager->setMating(true);
}

void EntityAIVillagerMate::resetTask()
{
    village = nullptr;
    mate = nullptr;
    villager->setMating(false);
}

bool EntityAIVillagerMate::shouldContinueExecuting()
{
    return matingTimeout >= 0 && checkSufficientDoorsPresentForNewVillager() && villager->getGrowingAge() == 0 && villager->getIsWillingToMate(false);
}

void EntityAIVillagerMate::updateTask()
{
    --matingTimeout;
    villager->getLookHelper().setLookPositionWithEntity(mate, 10.0F, 30.0F);
    if (villager->getDistanceSq(mate) > 2.25) 
    {
        villager->getNavigator().tryMoveToEntityLiving(mate, 0.25);
    }
    else if (matingTimeout == 0 && mate->isMating()) 
    {
        giveBirth();
    }

    if (villager->getRNG().nextInt(35) == 0) 
    {
        world->setEntityState(villager, 12);
    }
}

bool EntityAIVillagerMate::checkSufficientDoorsPresentForNewVillager() const
{
    if (!village->isMatingSeason()) 
    {
        return false;
    }
    else 
    {
        int i = (int)((double)((float)village->getNumVillageDoors()) * 0.35);
        return village->getNumVillagers() < i;
    }
}

void EntityAIVillagerMate::giveBirth()
{
    EntityVillager* entityvillager = villager->createChild(mate);
    mate->setGrowingAge(6000);
    villager->setGrowingAge(6000);
    mate->setIsWillingToMate(false);
    villager->setIsWillingToMate(false);
    entityvillager.setGrowingAge(-24000);
    entityvillager.setLocationAndAngles(villager->posX, villager->posY, villager->posZ, 0.0F, 0.0F);
    world->spawnEntity(entityvillager);
    world->setEntityState(entityvillager, 12);
}
