#pragma once
#include "WorldGenAbstractTree.h"

class WorldGenCanopyTree : public WorldGenAbstractTree
{
public:
	explicit WorldGenCanopyTree(bool notify);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* DARK_OAK_LOG;
	static IBlockState* DARK_OAK_LEAVES;

	bool placeTreeOfHeight(World* worldIn, const BlockPos& pos, int32_t height);
	void placeLogAt(World* worldIn, const BlockPos& pos);
	void placeLeafAt(World* worldIn, int32_t x, int32_t y, int32_t z);
};
