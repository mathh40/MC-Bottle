#pragma once
#include "EntityAIMoveToBlock.h"

class EntityOcelot;

class EntityAIOcelotSit :public EntityAIMoveToBlock
{
public:
    EntityAIOcelotSit(EntityOcelot* ocelotIn, double p_i45315_2_);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;
protected:
    bool shouldMoveTo(World* worldIn, BlockPos pos) override;
private:
    EntityOcelot* ocelot;
};