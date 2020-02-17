#pragma once
#include "WorldGenHugeTrees.h"

class WorldGenMegaPineTree : public WorldGenHugeTrees
{
public:
	WorldGenMegaPineTree(bool notify, bool useBaseHeight);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
	void generateSaplings(World* worldIn, pcg32& random, const BlockPos& pos) override;
private:
	static IBlockState* TRUNK;
	static IBlockState* LEAF;
	static IBlockState* PODZOL;
	bool useBaseHeight;
	void createCrown(World* worldIn, int32_t x, int32_t z, int32_t y, int32_t p_150541_5_, pcg32& rand);
	void placePodzolCircle(World* worldIn, const BlockPos& center);
	void placePodzolAt(World* worldIn, const BlockPos& pos);
};
