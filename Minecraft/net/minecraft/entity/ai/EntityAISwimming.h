#pragma once
#include "EntityAIBase.h"

class EntityLiving;

class EntityAISwimming : public EntityAIBase
{
  public:
    explicit EntityAISwimming(EntityLiving *entityIn);
    bool shouldExecute() override;
    void updateTask() override;

  private:
    EntityLiving *entity;
};
