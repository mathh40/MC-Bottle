#pragma once
#include "EntityAIAttackMelee.h"

class EntityZombie;
class EntityAIZombieAttack :public EntityAIAttackMelee
{
public:
    EntityAIZombieAttack(EntityZombie* zombieIn, double speedIn, bool longMemoryIn);
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    EntityZombie* zombie;
    int32_t raiseArmTicks;
};