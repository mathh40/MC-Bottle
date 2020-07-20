#pragma once
#include "EntityAITarget.h"

class EntityTameable;

class EntityAIOwnerHurtByTarget :public EntityAITarget
{
public:
    explicit EntityAIOwnerHurtByTarget(EntityTameable* theDefendingTameableIn);
    bool shouldExecute() override;
    void startExecuting() override;

private:
    EntityTameable* tameable;
    EntityLivingBase* attacker;
    int32_t timestamp;
};
