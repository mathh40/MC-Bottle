#pragma once
#include "EntityAIBase.h"

class EntityLivingBase;
class EntityLiving;
class IRangedAttackMob;

class EntityAIAttackRanged :public EntityAIBase
{
public:
    EntityAIAttackRanged(IRangedAttackMob* attacker, double movespeed, int32_t maxAttackTime, float maxAttackDistanceIn);
    EntityAIAttackRanged(IRangedAttackMob* attacker, double movespeed, int32_t p_i1650_4_, int32_t maxAttackTime, float maxAttackDistanceIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void resetTask() override;
    void updateTask() override;

private:
    EntityLiving* entityHost;
    IRangedAttackMob* rangedAttackEntityHost;
    EntityLivingBase* attackTarget;
    int32_t rangedAttackTime;
    double entityMoveSpeed;
    int32_t seeTime;
    int32_t attackIntervalMin;
    int32_t maxRangedAttackTime;
    float attackRadius;
    float maxAttackDistance;
};