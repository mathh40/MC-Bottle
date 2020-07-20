#pragma once
#include "EntityAITarget.h"

class EntityTameable;

class EntityAIOwnerHurtTarget :public EntityAITarget
{
public:
    explicit EntityAIOwnerHurtTarget(EntityTameable* theEntityTameableIn);
    bool shouldExecute() override;
    void startExecuting() override;
private:
    EntityTameable* tameable;
    EntityLivingBase* attacker;
    int32_t timestamp;
};
