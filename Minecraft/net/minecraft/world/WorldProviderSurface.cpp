#include "WorldProviderSurface.h"

DimensionType& WorldProviderSurface::getDimensionType()
{
	return DimensionType::OVERWORLD;
}

bool WorldProviderSurface::canDropChunk(int32_t x, int32_t z)
{
	return !world->isSpawnChunk(x, z);
}
