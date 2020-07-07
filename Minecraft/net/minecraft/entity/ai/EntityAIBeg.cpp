#include "EntityAIBeg.h"

#include "EnumHand.h"
#include "../../item/ItemStack.h"

EntityAIBeg::EntityAIBeg(EntityWolf *wolf, float minDistance)
    :wolf(wolf),world(wolf->world),minPlayerDistance(minDistance)
{
    setMutexBits(2);
}

bool EntityAIBeg::shouldExecute()
{
    player = world->getClosestPlayerToEntity(wolf, minPlayerDistance);
    return player == nullptr ? false : hasTemptationItemInHand(player);
}

bool EntityAIBeg::shouldContinueExecuting()
{
    if (!player->isEntityAlive()) 
    {
        return false;
    }
    else if (wolf->getDistanceSq(player) > minPlayerDistance * minPlayerDistance) 
    {
        return false;
    }
    else 
    {
        return timeoutCounter > 0 && hasTemptationItemInHand(player);
    }
}

void EntityAIBeg::startExecuting()
{
    wolf->setBegging(true);
    timeoutCounter = 40 + wolf->getRNG().nextInt(40);
}

void EntityAIBeg::resetTask()
{
    wolf->setBegging(false);
    player = nullptr;
}

void EntityAIBeg::updateTask()
{
    wolf->getLookHelper().setLookPosition(player->posX, player->posY + player->getEyeHeight(), player->posZ, 10.0F, wolf->getVerticalFaceSpeed());
    --timeoutCounter;
}

bool EntityAIBeg::hasTemptationItemInHand(EntityPlayer *player)
{
    for(auto& enumhand : {EnumHand::MAIN_HAND,EnumHand::OFF_HAND})
    {
        ItemStack itemstack = player->getHeldItem(enumhand);
        if (wolf->isTamed() && itemstack.getItem() == Items::BONE) 
        {
            return true;
        }

        if (wolf->isBreedingItem(itemstack)) 
        {
            return true;
        }
    }

    return false;
}
