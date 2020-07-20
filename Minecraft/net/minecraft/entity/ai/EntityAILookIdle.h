#pragma once
#include "EntityAIBase.h"

class EntityLiving;

class EntityAILookIdle :public EntityAIBase
{
public:
    explicit EntityAILookIdle(EntityLiving* entitylivingIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void updateTask() override;

private:
    EntityLiving* idleEntity;
    double lookX;
    double lookZ;
    int32_t idleTime;
};
