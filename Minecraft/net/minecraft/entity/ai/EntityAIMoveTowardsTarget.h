#pragma once
#include "EntityAIBase.h"

class EntityCreature;
class EntityLivingBase;

class EntityAIMoveTowardsTarget : public EntityAIBase
{
  public:
    EntityAIMoveTowardsTarget(EntityCreature *creature, double speedIn, float targetMaxDistance);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void resetTask() override;
    void startExecuting() override;

  private:
    EntityCreature *creature;
    EntityLivingBase *targetEntity;
    double movePosX;
    double movePosY;
    double movePosZ;
    double speed;
    float maxTargetDistance;
};
