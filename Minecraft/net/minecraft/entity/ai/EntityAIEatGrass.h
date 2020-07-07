#pragma once
#include "EntityAIBase.h"

class World;
class EntityLiving;
class EntityAIEatGrass :public EntityAIBase
{
public:
    EntityAIEatGrass(EntityLiving* grassEaterEntityIn);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;
    bool shouldContinueExecuting() override;
    int32_t getEatingGrassTimer() const;
    void updateTask() override;


private:
    EntityLiving* grassEaterEntity;
    World* entityWorld;
    int32_t eatingGrassTimer;
};