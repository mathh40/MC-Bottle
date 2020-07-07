#include "EntityAIBase.h"

bool EntityAIBase::shouldContinueExecuting()
{
    return shouldExecute();
}

bool EntityAIBase::isInterruptible()
{
    return true;
}

void EntityAIBase::startExecuting()
{
}

void EntityAIBase::resetTask()
{
}

void EntityAIBase::updateTask()
{
}

void EntityAIBase::setMutexBits(int32_t mutexBitsIn)
{
    mutexBits = mutexBitsIn;
}

int32_t EntityAIBase::getMutexBits() const
{
    return mutexBits;
}
