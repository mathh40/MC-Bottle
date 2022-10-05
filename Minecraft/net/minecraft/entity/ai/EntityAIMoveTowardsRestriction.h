#pragma once
#include "EntityAIBase.h"

class EntityCreature;

class EntityAIMoveTowardsRestriction : public EntityAIBase
{
  public:
    EntityAIMoveTowardsRestriction(EntityCreature *creatureIn, double speedIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;

  private:
    EntityCreature *creature;
    double movePosX{};
    double movePosY{};
    double movePosZ{};
    double movementSpeed;
};
