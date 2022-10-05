#pragma once
#include "../../../entity/item/EntityMinecartContainer.h"
#include "ResourceLocation.h"

class ILootContainer
{
  public:
    virtual ~ILootContainer()                              = default;
    virtual std::optional<ResourceLocation> getLootTable() = 0;
};
