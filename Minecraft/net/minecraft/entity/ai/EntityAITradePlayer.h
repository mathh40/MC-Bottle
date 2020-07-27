#pragma once
#include "EntityAIBase.h"

class EntityVillager;

class EntityAITradePlayer :public EntityAIBase
{
public:
    explicit EntityAITradePlayer(EntityVillager* villagerIn);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;
private:
    EntityVillager* villager;
};
