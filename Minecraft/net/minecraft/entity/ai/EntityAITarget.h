#pragma once
#include "EntityAIBase.h"

class EntityLiving;
class EntityCreature;
class EntityLivingBase;
class EntityAITarget :public EntityAIBase
{
public:
    EntityAITarget(EntityCreature* creature, bool checkSight);
    EntityAITarget(EntityCreature* creature, bool checkSight, bool onlyNearby);
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    static bool isSuitableTarget(EntityLiving* attacker, EntityLivingBase* target, bool includeInvincibles, bool checkSight);
    EntityAITarget* setUnseenMemoryTicks(int32_t p_190882_1_);

protected:
    double getTargetDistance();
    bool isSuitableTarget(EntityLivingBase* target, bool includeInvincibles);

    EntityCreature* taskOwner;
    bool shouldCheckSight;
    EntityLivingBase* target;
    int32_t unseenMemoryTicks;
private:
    bool canEasilyReach(EntityLivingBase* target);

    bool nearbyOnly;
    int32_t targetSearchStatus;
    int32_t targetSearchDelay;
    int32_t targetUnseenTicks;
};