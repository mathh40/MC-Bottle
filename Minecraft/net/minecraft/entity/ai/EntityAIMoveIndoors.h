#pragma once
#include "../../village/VillageDoorInfo.h"
#include "EntityAIBase.h"

class EntityCreature;

class EntityAIMoveIndoors : public EntityAIBase
{
  public:
    EntityAIMoveIndoors(EntityCreature *entityIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;

  private:
    EntityCreature *entity;
    VillageDoorInfo doorInfo;
    int32_t insidePosX = -1;
    int32_t insidePosZ = -1;
};
