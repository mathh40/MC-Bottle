#pragma once
#include "WorldGenerator.h"
#include "Block.h"

class WorldGenBlockBlob : public WorldGenerator
{
public:
	WorldGenBlockBlob(Block* blockIn, int32_t startRadiusIn);

	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	Block* block;
	int32_t startRadius;

};
