#pragma once
#include "WorldGenAbstractTree.h"

class WorldGenTaiga1 : public WorldGenAbstractTree
{
public:
	WorldGenTaiga1();
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* TRUNK;
	static IBlockState* LEAF;
};