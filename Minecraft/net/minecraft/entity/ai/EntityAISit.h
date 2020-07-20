#pragma once
#include "EntityAIBase.h"

class EntityTameable;

class EntityAISit :public EntityAIBase
{
public:
    explicit EntityAISit(EntityTameable* entityIn);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;
    void setSitting(bool sitting);

private:
    EntityTameable* tameable;
    bool isSitting;
};
