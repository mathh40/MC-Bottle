#pragma once
#include "EntityAIMoveToBlock.h"

class EntityVillager;
class EntityAIHarvestFarmland :public EntityAIMoveToBlock
{
public:
    EntityAIHarvestFarmland(EntityVillager* villagerIn, double speedIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void updateTask() override;

protected:
    bool shouldMoveTo(World* worldIn, BlockPos pos) override;

private:
    EntityVillager* villager;
    bool hasFarmItem;
    bool wantsToReapStuff;
    int32_t currentTask;
};