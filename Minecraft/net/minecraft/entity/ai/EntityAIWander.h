#pragma once
#include "EntityAIBase.h"
#include "math/Vec3d.h"

class EntityCreature;

class EntityAIWander :public EntityAIBase
{
public:
    EntityAIWander(EntityCreature* creatureIn, double speedIn);
    EntityAIWander(EntityCreature* creatureIn, double speedIn, int32_t chance);

    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void makeUpdate();
    void setExecutionChance(int32_t newchance);

protected:
    virtual std::optional<Vec3d> getPosition();

    EntityCreature* entity;
    double x;
    double y;
    double z;
    double speed;
    int32_t executionChance;
    bool mustUpdate;
};
