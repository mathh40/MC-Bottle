#pragma once
#include <functional>

#include "EntityAIBase.h"

class PathNavigate;
class EntityLiving;
class EntityAIFollow :public EntityAIBase
{
public:
    EntityAIFollow(EntityLiving* p_i47417_1_, double p_i47417_2_, float p_i47417_4_, float p_i47417_5_);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    EntityLiving* entity;
    std::function<bool(EntityLiving*)> followPredicate;
    EntityLiving* followingEntity;
    double speedModifier;
    PathNavigate* navigation;
    int32_t timeToRecalcPath;
    float stopDistance;
    float oldWaterCost;
    float areaSize;
};
