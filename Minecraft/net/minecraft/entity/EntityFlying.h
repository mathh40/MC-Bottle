#pragma once
#include "EntityLiving.h"

class EntityFlying :public EntityLiving {
public:
    EntityFlying(World* worldIn);
    void fall(float distance, float damageMultiplier) override;
    void travel(float strafe, float vertical, float forward) override;
    bool isOnLadder() override;

protected:
    void updateFallState(double y, bool onGroundIn, IBlockState* state, BlockPos pos);
private:
};