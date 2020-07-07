#pragma once
#include "EntityAIBase.h"

class EntityIronGolem;
class EntityVillager;
class EntityAIFollowGolem :public EntityAIBase
{
public:
    explicit EntityAIFollowGolem(EntityVillager* villagerIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    EntityVillager* villager;
    EntityIronGolem* ironGolem;
    int32_t takeGolemRoseTick;
    bool tookGolemRose;
};
