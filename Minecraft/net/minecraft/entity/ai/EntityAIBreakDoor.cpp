#include "EntityAIBreakDoor.h"

#include "Block.h"
#include "EnumDifficulty.h"

EntityAIBreakDoor::EntityAIBreakDoor(EntityLiving *entityIn) : EntityAIDoorInteract(entityIn)
{
}

bool EntityAIBreakDoor::shouldExecute()
{
    if (!EntityAIDoorInteract::shouldExecute())
    {
        return false;
    }
    else if (!entity->world.getGameRules().getBoolean("mobGriefing"))
    {
        return false;
    }
    else
    {
        BlockDoor *blockdoor = doorBlock;
        return !BlockDoor::isOpen(entity->world, doorPosition);
    }
}

void EntityAIBreakDoor::startExecuting()
{
    EntityAIDoorInteract::startExecuting();
    breakingTime = 0;
}

bool EntityAIBreakDoor::shouldContinueExecuting()
{
    double d0 = entity->getDistanceSq(doorPosition);
    bool flag;
    if (breakingTime <= 240)
    {
        BlockDoor *blockdoor = doorBlock;
        if (!BlockDoor::isOpen(entity->world, doorPosition) && d0 < 4.0)
        {
            flag = true;
            return flag;
        }
    }

    flag = false;
    return flag;
}

void EntityAIBreakDoor::resetTask()
{
    EntityAIDoorInteract::resetTask();
    entity->world.sendBlockBreakProgress(entity->getEntityId(), doorPosition, -1);
}

void EntityAIBreakDoor::updateTask()
{
    EntityAIDoorInteract::updateTask();
    if (entity->getRNG().nextInt(20) == 0)
    {
        entity->world.playEvent(1019, doorPosition, 0);
    }

    ++breakingTime;
    auto i = breakingTime / 240.0F * 10.0F);
    if (i != previousBreakProgress)
    {
        entity->world.sendBlockBreakProgress(entity->getEntityId(), doorPosition, i);
        previousBreakProgress = i;
    }

    if (breakingTime == 240 && entity->world.getDifficulty() == EnumDifficulty::HARD)
    {
        entity->world.setBlockToAir(doorPosition);
        entity->world.playEvent(1021, doorPosition, 0);
        entity->world.playEvent(2001, doorPosition, Block::getIdFromBlock(doorBlock));
    }
}
