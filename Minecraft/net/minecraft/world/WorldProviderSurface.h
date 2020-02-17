#pragma once
#include "WorldProvider.h"

class WorldProviderSurface :public WorldProvider
{
public:
	DimensionType& getDimensionType() override;
	bool canDropChunk(int32_t x, int32_t z) override;
private:
};
