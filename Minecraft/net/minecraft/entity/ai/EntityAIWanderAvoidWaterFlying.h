#pragma once
#include "EntityAIWanderAvoidWater.h"

class EntityAIWanderAvoidWaterFlying :public EntityAIWanderAvoidWater
{
public:
    EntityAIWanderAvoidWaterFlying(EntityCreature* p_i47413_1_, double p_i47413_2_);
protected:
    std::optional<Vec3d> getPosition() override;
private:
    std::optional<Vec3d> getTreePos();
};