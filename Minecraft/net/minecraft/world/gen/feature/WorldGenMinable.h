#pragma once
#include "WorldGenerator.h"
#include <functional>

bool StonePredicate(IBlockState* p_apply_1_)
{
	if (p_apply_1_ != nullptr && p_apply_1_->getBlock() == Blocks::STONE) 
	{
		BlockStone::EnumType blockstone$enumtype = (BlockStone::EnumType)p_apply_1_->getValue(BlockStone::VARIANT);
		return blockstone$enumtype.isNatural();
	}
	else 
	{
		return false;
	}
};

class WorldGenMinable : public WorldGenerator
{
public:
	WorldGenMinable(IBlockState* state, int32_t blockCount);
	WorldGenMinable(IBlockState* state, int32_t blockCount, std::function<bool(IBlockState*)> p_i45631_3_);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	IBlockState* oreBlock;
	int32_t numberOfBlocks;
	std::function<bool(IBlockState*)> predicate;
};