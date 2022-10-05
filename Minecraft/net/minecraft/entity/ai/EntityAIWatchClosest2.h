#pragma once
#include "EntityAIWatchClosest.h"

class EntityAIWatchClosest2 : public EntityAIWatchClosest
{
  public:
    EntityAIWatchClosest2(EntityLiving *entityIn, std::type_index watchTargetClass, float maxDistance, float chanceIn);
};
