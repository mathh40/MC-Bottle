#pragma once
#include "WorldGenerator.h"
#include "Block.h"

class WorldGenIcePath : public WorldGenerator
{
public:
	WorldGenIcePath(int32_t basePathWidthIn);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	Block* block;
	int32_t basePathWidth;
};
