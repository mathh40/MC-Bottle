#pragma once
#include "EntityAIBase.h"
#include "math/BlockPos.h"

class EntityLiving;
class BlockDoor;
class EntityAIDoorInteract : public EntityAIBase
{
  public:
    EntityAIDoorInteract(EntityLiving *entityIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void updateTask() override;

  protected:
    EntityLiving *entity;
    BlockPos doorPosition;
    BlockDoor *doorBlock;

  private:
    BlockDoor *getBlockDoor(BlockPos pos);

    bool hasStoppedDoorInteraction;
    float entityPositionX;
    float entityPositionZ;
};
