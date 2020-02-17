#pragma once
#include "WorldGenAbstractTree.h"

class WorldGenBirchTree :public WorldGenAbstractTree
{
public:
	WorldGenBirchTree(bool notify, bool useExtraRandomHeightIn);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* LOG;
	static IBlockState* LEAF;
	bool useExtraRandomHeight;
};
