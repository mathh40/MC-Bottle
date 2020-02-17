#pragma once
#include "WorldGenTrees.h"

class WorldGenShrub : public WorldGenTrees
{
public:
	WorldGenShrub(IBlockState* woodMetadataIn, IBlockState* leavesMetadataIn);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	IBlockState* leavesMetadata;
	IBlockState* woodMetadata;
};
