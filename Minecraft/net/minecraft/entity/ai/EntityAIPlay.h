#pragma once
#include "EntityAIBase.h"

class EntityVillager;
class EntityLivingBase;

class EntityAIPlay : public EntityAIBase
{
  public:
    EntityAIPlay(EntityVillager *villagerIn, double speedIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    EntityVillager *villager;
    EntityLivingBase *targetVillager;
    double speed;
    int32_t playTime;
};
