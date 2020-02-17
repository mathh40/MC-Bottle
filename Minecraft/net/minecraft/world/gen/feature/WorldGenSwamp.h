#pragma once
#include "WorldGenAbstractTree.h"

class PropertyBool;

class WorldGenSwamp : public WorldGenAbstractTree
{
public:
	WorldGenSwamp();
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* TRUNK;
	static IBlockState* LEAF;

	void addVine(World* worldIn, const BlockPos& pos, PropertyBool* prop);
};
