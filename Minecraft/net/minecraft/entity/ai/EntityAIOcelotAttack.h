#pragma once
#include "EntityAIBase.h"

class EntityLivingBase;
class EntityLiving;
class World;

class EntityAIOcelotAttack : public EntityAIBase
{
  public:
    explicit EntityAIOcelotAttack(EntityLiving *theEntityIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    World *world;
    EntityLiving *entity;
    EntityLivingBase *target;
    int32_t attackCountdown;
};
