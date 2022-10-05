#pragma once
#include "EntityAIBase.h"
#include "math/BlockPos.h"

#include <optional>

class World;
class EntityCreature;

class EntityAIPanic : public EntityAIBase
{
  public:
    EntityAIPanic(EntityCreature *creature, double speedIn);
    bool shouldExecute() override;
    void startExecuting() override;
    bool shouldContinueExecuting() override;

  protected:
    bool findRandomPosition();

    EntityCreature *creature;
    double speed;
    double randPosX;
    double randPosY;
    double randPosZ;

  private:
    std::optional<BlockPos> getRandPos(World *worldIn, Entity *entityIn, int32_t horizontalRange,
                                       int32_t verticalRange);
};
