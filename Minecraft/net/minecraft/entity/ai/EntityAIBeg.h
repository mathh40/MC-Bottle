#pragma once
#include "EntityAIBase.h"

class World;
class EntityPlayer;
class EntityWolf;
class EntityAIBeg : public EntityAIBase
{
  public:
    EntityAIBeg(EntityWolf *wolf, float minDistance);
    bool shouldExecute() override;
    bool shouldContinueExecuting() override;
    void startExecuting() override;
    void resetTask() override;
    void updateTask() override;

  private:
    bool hasTemptationItemInHand(EntityPlayer *player);

    EntityWolf *wolf;
    EntityPlayer *player;
    World *world;
    float minPlayerDistance;
    int32_t timeoutCounter;
};
