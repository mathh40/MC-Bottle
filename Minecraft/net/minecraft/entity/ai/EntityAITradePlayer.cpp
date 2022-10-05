#include "EntityAITradePlayer.h"

class EntityPlayer;

EntityAITradePlayer::EntityAITradePlayer(EntityVillager *villagerIn) : villager(villagerIn)
{
    setMutexBits(5);
}

bool EntityAITradePlayer::shouldExecute()
{
    if (!villager->isEntityAlive())
    {
        return false;
    }
    else if (villager->isInWater())
    {
        return false;
    }
    else if (!villager->onGround)
    {
        return false;
    }
    else if (villager->velocityChanged)
    {
        return false;
    }
    else
    {
        EntityPlayer *entityplayer = villager->getCustomer();
        if (entityplayer == nullptr)
        {
            return false;
        }
        else if (villager->getDistanceSq(entityplayer) > 16.0)
        {
            return false;
        }
        else
        {
            return entityplayer->openContainer != nullptr;
        }
    }
}

void EntityAITradePlayer::startExecuting()
{
    villager->getNavigator().clearPath();
}

void EntityAITradePlayer::resetTask()
{
    villager->setCustomer(nullptr);
}
