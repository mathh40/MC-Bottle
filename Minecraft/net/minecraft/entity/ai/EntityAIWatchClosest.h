#pragma once
#include <typeindex>

#include "EntityAIBase.h"

class EntityLiving;
class Entity;

class EntityAIWatchClosest :public EntityAIBase
{
public:
    EntityAIWatchClosest(EntityLiving* entityIn, std::type_index watchTargetClass, float maxDistance);
    EntityAIWatchClosest(EntityLiving* entityIn, std::type_index watchTargetClass, float maxDistance, float chanceIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

protected:
    EntityLiving* entity;
    Entity* closestEntity;
    float maxDistance;
    std::type_index watchedClass;
private:
    int lookTime;
    float chance;
};
