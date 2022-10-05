#pragma once
#include "EntityAIDoorInteract.h"

class EntityAIBreakDoor : public EntityAIDoorInteract
{
  public:
    EntityAIBreakDoor(EntityLiving *entityIn);
    bool shouldExecute() override;
    void startExecuting() override;
    bool shouldContinueExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    int32_t breakingTime;
    int32_t previousBreakProgress = -1;
};
