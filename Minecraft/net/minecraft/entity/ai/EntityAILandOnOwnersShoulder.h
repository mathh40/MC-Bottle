#pragma once
#include "EntityAIBase.h"

class EntityPlayer;

class EntityAILandOnOwnersShoulder : public EntityAIBase
{
  public:
    EntityAILandOnOwnersShoulder(EntityShoulderRiding *entityIn);
    bool shouldExecute() override;
    bool isInterruptible() override;
    void startExecuting() override;
    void updateTask() override;

  private:
    EntityShoulderRiding *entity;
    EntityPlayer *owner;
    bool isSittingOnShoulder;
};
