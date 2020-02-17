#pragma once
#include "WorldGenerator.h"

class WorldGenMelon : public WorldGenerator
{
public:
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
};
