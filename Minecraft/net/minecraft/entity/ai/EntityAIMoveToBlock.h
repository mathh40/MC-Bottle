#pragma once
#include "EntityAIBase.h"
#include "math/BlockPos.h"

class World;
class EntityCreature;
class EntityAIMoveToBlock : public EntityAIBase
{
  public:
    EntityAIMoveToBlock(EntityCreature *creature, double speedIn, int32_t length);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void updateTask() override;

  protected:
    bool getIsAboveDestination() const;
    virtual bool shouldMoveTo(World *var1, BlockPos var2) = 0;

    int32_t runDelay;
    BlockPos destinationBlock;

  private:
    bool searchForDestination();

    EntityCreature *creature;
    double movementSpeed;
    int32_t timeoutCounter;
    int32_t maxStayTicks;
    bool isAboveDestination;
    int32_t searchLength;
};
