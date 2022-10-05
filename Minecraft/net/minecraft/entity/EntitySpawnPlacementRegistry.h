#pragma once
#include "EntityLiving.h"

#include <unordered_map>

class EntitySpawnPlacementRegistry
{
  public:
    EntitySpawnPlacementRegistry();

    static SpawnPlacementType getPlacementForEntity(std::type_index entityClass);

  private:
    static std::unordered_map<std::type_index, SpawnPlacementType> ENTITY_PLACEMENTS;
};
