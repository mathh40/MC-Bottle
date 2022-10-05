#pragma once
#include "EntityAIBase.h"

class EntityIronGolem;
class EntityVillager;

class EntityAILookAtVillager : public EntityAIBase
{
  public:
    explicit EntityAILookAtVillager(EntityIronGolem *ironGolemIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    EntityIronGolem *ironGolem;
    EntityVillager *villager;
    int32_t lookTime;
};
