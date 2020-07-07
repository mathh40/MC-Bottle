#pragma once
#include "EntityAIBase.h"
#include "math/Vec3d.h"

class World;
class EntityCreature;
class EntityAIFleeSun :public EntityAIBase
{
public:
    EntityAIFleeSun(EntityCreature* theCreatureIn, double movementSpeedIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;

private:
    std::optional<Vec3d> findPossibleShelter();

    EntityCreature* creature;
    double shelterX;
    double shelterY;
    double shelterZ;
    double movementSpeed;
    World* world;
};