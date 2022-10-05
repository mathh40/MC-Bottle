#pragma once
#include "EntityAIBase.h"

class EntityLivingBase;
class World;
class EntityCreature;
class Path;

class EntityAIAttackMelee : public EntityAIBase
{
  public:
    EntityAIAttackMelee(EntityCreature *creature, double speedIn, bool useLongMemory);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

  protected:
    void checkAndPerformAttack(EntityLivingBase *enemy, double distToEnemySqr);
    double getAttackReachSqr(EntityLivingBase *attackTarget) const;

    EntityCreature *attacker;
    Path *path;
    int32_t attackTick;
    int32_t attackInterval = 20;

  private:
    World *world;
    double speedTowardsTarget;
    bool longMemory;
    int32_t delayCounter;
    double targetX;
    double targetY;
    double targetZ;
};
