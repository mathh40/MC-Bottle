#pragma once
#include "ResourceLocation.h"
#include "../../../entity/item/EntityMinecartContainer.h"
#include "../../../entity/item/EntityMinecartContainer.h"

class ILootContainer 
{
	public:
    virtual ~ILootContainer() = default;
    virtual std::optional<ResourceLocation> getLootTable() = 0;
};
