#pragma once
#include "EntityAIBase.h"

class World;
class EntityLivingBase;
class EntityTameable;
class PathNavigate;
class EntityAIFollowOwner :public EntityAIBase
{
public:
    EntityAIFollowOwner(EntityTameable* tameableIn, double followSpeedIn, float minDistIn, float maxDistIn);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

protected:
    virtual bool isTeleportFriendlyBlock(int32_t x, int32_t z, int32_t y, int32_t xOffset, int32_t zOffset);

private:
    EntityTameable* tameable;
    EntityLivingBase* owner;
    World* world;
    PathNavigate* petPathfinder;
    int32_t timeToRecalcPath;
    float maxDist;
    float minDist;
    float oldWaterCost;
    double followSpeed;
};
