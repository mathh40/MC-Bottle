#pragma once
#include "EntityAIBase.h"

class EntityLiving;
class EntityLivingBase;

class EntityAILeapAtTarget :public EntityAIBase
{
public:
    EntityAILeapAtTarget(EntityLiving *leapingEntity, float leapMotionYIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;

private:
    EntityLiving* leaper;
    EntityLivingBase* leapTarget;
    float leapMotionY;
};
