#include "EntityAIMoveToBlock.h"

EntityAIMoveToBlock::EntityAIMoveToBlock(EntityCreature *creature, double speedIn, int32_t length)
    : destinationBlock(BlockPos::ORIGIN), creature(creature), movementSpeed(speedIn), searchLength(length)
{
    setMutexBits(5);
}

bool EntityAIMoveToBlock::shouldExecute()
{
    if (runDelay > 0)
    {
        --runDelay;
        return false;
    }
    else
    {
        runDelay = 200 + creature->getRNG()(200);
        return searchForDestination();
    }
}

bool EntityAIMoveToBlock::shouldContinueExecuting()
{
    return timeoutCounter >= -maxStayTicks && timeoutCounter <= 1200 && shouldMoveTo(creature->world, destinationBlock);
}

void EntityAIMoveToBlock::startExecuting()
{
    creature->getNavigator().tryMoveToXYZ(destinationBlock.getx() + 0.5, destinationBlock.gety() + 1,
                                          destinationBlock.getx() + 0.5, movementSpeed);
    timeoutCounter = 0;
    maxStayTicks   = creature->getRNG()(creature->getRNG()(1200) + 1200) + 1200;
}

void EntityAIMoveToBlock::updateTask()
{
    if (creature->getDistanceSqToCenter(destinationBlock.up()) > 1.0)
    {
        isAboveDestination = false;
        ++timeoutCounter;
        if (timeoutCounter % 40 == 0)
        {
            creature->getNavigator().tryMoveToXYZ(destinationBlock.getx() + 0.5, destinationBlock.gety() + 1,
                                                  destinationBlock.getz() + 0.5, movementSpeed);
        }
    }
    else
    {
        isAboveDestination = true;
        --timeoutCounter;
    }
}

bool EntityAIMoveToBlock::getIsAboveDestination() const
{
    return isAboveDestination;
}

bool EntityAIMoveToBlock::searchForDestination()
{
    auto i            = searchLength;
    BlockPos blockpos = BlockPos(creature);

    for (auto k = 0; k <= 1; k = k > 0 ? -k : 1 - k)
    {
        for (auto l = 0; l < i; ++l)
        {
            for (auto i1 = 0; i1 <= l; i1 = i1 > 0 ? -i1 : 1 - i1)
            {
                for (auto j1 = i1 < l && i1 > -l ? l : 0; j1 <= l; j1 = j1 > 0 ? -j1 : 1 - j1)
                {
                    BlockPos blockpos1 = blockpos.add(i1, k - 1, j1);
                    if (creature->isWithinHomeDistanceFromPosition(blockpos1) &&
                        shouldMoveTo(creature->world, blockpos1))
                    {
                        destinationBlock = blockpos1;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
