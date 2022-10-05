#include "EntityAIZombieAttack.h"

EntityAIZombieAttack::EntityAIZombieAttack(EntityZombie *zombieIn, double speedIn, bool longMemoryIn)
    : EntityAIAttackMelee(zombieIn, speedIn, longMemoryIn), zombie(zombieIn)
{
}

void EntityAIZombieAttack::startExecuting()
{
    EntityAIAttackMelee::startExecuting();
    raiseArmTicks = 0;
}

void EntityAIZombieAttack::resetTask()
{
    EntityAIAttackMelee::resetTask();
    zombie->setArmsRaised(false);
}

void EntityAIZombieAttack::updateTask()
{
    EntityAIAttackMelee::updateTask();
    ++raiseArmTicks;
    if (raiseArmTicks >= 5 && attackTick < 10)
    {
        zombie->setArmsRaised(true);
    }
    else
    {
        zombie->setArmsRaised(false);
    }
}
