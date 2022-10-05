#pragma once
#include "EntityAIWander.h"

class EntityAIWanderAvoidWater : public EntityAIWander
{
  public:
    EntityAIWanderAvoidWater(EntityCreature *p_i47301_1_, double p_i47301_2_);
    EntityAIWanderAvoidWater(EntityCreature *p_i47302_1_, double p_i47302_2_, float p_i47302_4_);

  protected:
    std::optional<Vec3d> getPosition() override;
    float probability;
};
