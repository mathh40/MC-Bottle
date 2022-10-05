#pragma once
#include "EntityAIBase.h"

class EntityAnimal;
class EntityAIFollowParent : public EntityAIBase
{
  public:
    EntityAIFollowParent(EntityAnimal *animal, double speed);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    EntityAnimal *childAnimal;
    EntityAnimal *parentAnimal;
    double moveSpeed;
    int32_t delayCounter;
};
