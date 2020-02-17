#pragma once
#include "WorldGenerator.h"

class WorldGenTallGrass : public WorldGenerator
{
public:
	WorldGenTallGrass(BlockTallGrass::EnumType p_i45629_1_);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	IBlockState* tallGrassState;
};