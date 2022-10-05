#pragma once
#include "EntityAIFollowOwner.h"

class EntityAIFollowOwnerFlying : public EntityAIFollowOwner
{
  public:
    EntityAIFollowOwnerFlying(EntityTameable *tameableIn, double followSpeedIn, float minDistIn, float maxDistIn);

  protected:
    bool isTeleportFriendlyBlock(int32_t x, int32_t z, int32_t y, int32_t xOffset, int32_t zOffset) override;
};
