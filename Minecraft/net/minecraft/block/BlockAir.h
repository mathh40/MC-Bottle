#pragma once
#include "Block.h"

class BlockAir :public Block
{
public:
	EnumBlockRenderType getRenderType(IBlockState state) override;
	std::optional<AxisAlignedBB> getCollisionBoundingBox(IBlockState blockState, IBlockAccess worldIn, BlockPos pos)override;
	bool isOpaqueCube(IBlockState state)override;
	bool canCollideCheck(IBlockState state, bool hitIfLiquid)override;
	void dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState state, float chance, int32_t fortune)override;
	bool isReplaceable(IBlockAccess worldIn, BlockPos pos)override;
	bool isFullCube(IBlockState state)override;
	BlockFaceShape getBlockFaceShape(IBlockAccess worldIn, IBlockState state, BlockPos pos, EnumFacing face)override;
protected:
	~BlockAir() = default;
	BlockAir();
private:
};
