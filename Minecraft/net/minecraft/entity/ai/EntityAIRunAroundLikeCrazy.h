#pragma once
#include "EntityAIBase.h"

class AbstractHorse;

class EntityAIRunAroundLikeCrazy :public EntityAIBase
{
public:
    EntityAIRunAroundLikeCrazy(AbstractHorse* horse, double speedIn);
    bool shouldExecute() override;
    void startExecuting() override;
    bool shouldContinueExecuting() override;
    void updateTask() override;

private:
    AbstractHorse* horseHost;
    double speed;
    double targetX;
    double targetY;
    double targetZ;
};
