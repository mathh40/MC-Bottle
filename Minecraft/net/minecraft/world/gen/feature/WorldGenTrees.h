#pragma once
#include "WorldGenAbstractTree.h"

class PropertyBool;
class EnumFacing;

class WorldGenTrees : public WorldGenAbstractTree
{
public:
	WorldGenTrees(bool notify);
	WorldGenTrees(bool notify, int32_t minTreeHeightIn, IBlockState* woodMeta, IBlockState* p_i46446_4_, bool growVines);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static IBlockState* DEFAULT_TRUNK;
	static IBlockState* DEFAULT_LEAF;
	int32_t minTreeHeight;
	bool vinesGrow;
	IBlockState* metaWood;
	IBlockState* metaLeaves;

	void placeCocoa(World* worldIn, int32_t p_181652_2_, const BlockPos& pos, EnumFacing side);
	void addVine(World* worldIn, const BlockPos& pos, PropertyBool* prop);
	void addHangingVine(World* worldIn, const BlockPos& pos, PropertyBool* prop);
};
