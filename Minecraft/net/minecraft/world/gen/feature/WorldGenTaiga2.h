#pragma once
#include "WorldGenAbstractTree.h"

class WorldGenTaiga2 : public WorldGenAbstractTree
{
public:
	WorldGenTaiga2(bool notif);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* TRUNK;
	static IBlockState* LEAF;
};