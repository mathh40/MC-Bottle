#pragma once
#include "EntityLiving.h"

class EntityCreature : public EntityLiving
{
  public:
    EntityCreature(World *worldIn);
    virtual float getBlockPathWeight(BlockPos pos);
    bool getCanSpawnHere() override;
    bool hasPath() const;
    bool isWithinHomeDistanceCurrentPosition() const;
    bool isWithinHomeDistanceFromPosition(BlockPos pos) const;
    void setHomePosAndDistance(BlockPos pos, int32_t distance);
    BlockPos getHomePosition() const;
    float getMaximumHomeDistance() const;
    void detachHome();
    bool hasHome() const;

    static xg::Guid FLEEING_SPEED_MODIFIER_UUID = xg::Guid("E199AD21-BA8A-4C53-8D13-6182D5C69D3A");
    static AttributeModifier FLEEING_SPEED_MODIFIER;

  protected:
    void updateLeashedState() override;
    double followLeashSpeed();
    void onLeashDistance(float p_142017_1_);

  private:
    BlockPos homePosition;
    float maximumHomeDistance;
    float restoreWaterCost;
};
