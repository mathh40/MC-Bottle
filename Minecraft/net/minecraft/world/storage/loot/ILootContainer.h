#pragma once
#include "ResourceLocation.h"

class ILootContainer 
{
	public:
    virtual ~ILootContainer() = default;
    virtual ResourceLocation getLootTable() = 0;
};
