#pragma once
#include "WorldGenAbstractTree.h"

class WorldGenSavannaTree : public WorldGenAbstractTree
{
public:
	explicit WorldGenSavannaTree(bool doBlockNotify);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* TRUNK;
	static IBlockState* LEAF;
	void placeLogAt(World* worldIn, const BlockPos& pos);
	void placeLeafAt(World* worldIn, const BlockPos& pos);
};