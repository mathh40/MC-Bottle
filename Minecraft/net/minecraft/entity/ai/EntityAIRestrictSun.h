#pragma once
#include "EntityAIBase.h"

class EntityCreature;

class EntityAIRestrictSun : public EntityAIBase
{
  public:
    explicit EntityAIRestrictSun(EntityCreature *creature);
    bool shouldExecute() override;
    void startExecuting() override;
    void resetTask() override;

  private:
    EntityCreature *entity;
};
