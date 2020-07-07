#pragma once
#include "EntityAIBase.h"
class EntityMob;
class EntityAIAttackRangedBow :public EntityAIBase
{
public:
    EntityAIAttackRangedBow(EntityMob* mob, double moveSpeedAmpIn, int attackCooldownIn, float maxAttackDistanceIn);
    void setAttackCooldown(int32_t p_189428_1_);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

protected:
    bool isBowInMainhand();

private:
    EntityMob* entity;
    double moveSpeedAmp;
    int32_t attackCooldown;
    float maxAttackDistance;
    int32_t attackTime = -1;
    int32_t seeTime;
    bool strafingClockwise;
    bool strafingBackwards;
    int32_t strafingTime = -1;
};