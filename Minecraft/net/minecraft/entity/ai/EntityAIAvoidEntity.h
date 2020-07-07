#pragma once
#include <functional>
#include <typeindex>
#include "EntityAIBase.h"

class Entity;
class Path;
class PathNavigate;
class EntityCreature;

class EntityAIAvoidEntity :public EntityAIBase
{
public:
    EntityAIAvoidEntity(EntityCreature* entityIn, std::type_index classToAvoidIn, float avoidDistanceIn, double farSpeedIn, double nearSpeedIn);
    EntityAIAvoidEntity(EntityCreature* entityIn, std::type_index classToAvoidIn, std::function<bool(Entity* p_apply_1_)> avoidTargetSelectorIn, float avoidDistanceIn, double farSpeedIn, double nearSpeedIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

protected:
    EntityCreature* entity;
    Entity* closestLivingEntity;
private:
    std::function<bool(Entity* p_apply_1_)> canBeSeenSelector;
    double farSpeed;
    double nearSpeed;
    float avoidDistance;
    Path* path;
    PathNavigate* navigation;
    std::type_index classToAvoid;
    std::function<bool(Entity* p_apply_1_)> avoidTargetSelector;
};