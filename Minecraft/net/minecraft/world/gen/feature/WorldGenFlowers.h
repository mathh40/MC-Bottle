#pragma once
#include "WorldGenerator.h"

class BlockFlower;
class WorldGenFlowers :public WorldGenerator
{
public:
	WorldGenFlowers(BlockFlower* flowerIn, BlockFlower::EnumFlowerType type);
	void setGeneratedBlock(BlockFlower* flowerIn, BlockFlower::EnumFlowerType typeIn);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	BlockFlower* flower;
	IBlockState* state;
};
