#pragma once
#include "WorldGenerator.h"
#include "Block.h"

class WorldGenHellLava : public WorldGenerator
{
public:
	WorldGenHellLava(Block* blockIn, bool insideRockIn);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	Block* block;
	bool insideRock;
};
