#include "BlockAir.h"

BlockAir::BlockAir() : Block(Material.AIR)
{
}

EnumBlockRenderType BlockAir::getRenderType(IBlockState state)
{
    return EnumBlockRenderType::INVISIBLE;
}

std::optional<AxisAlignedBB> BlockAir::getCollisionBoundingBox(IBlockState blockState, IBlockAccess worldIn,
                                                               BlockPos pos)
{
    return std::nullopt;
}

bool BlockAir::isOpaqueCube(IBlockState state)
{
    return false;
}

bool BlockAir::canCollideCheck(IBlockState state, bool hitIfLiquid)
{
    return false;
}

void BlockAir::dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState state, float chance, int32_t fortune)
{
}

bool BlockAir::isReplaceable(IBlockAccess worldIn, BlockPos pos)
{
    return true;
}

bool BlockAir::isFullCube(IBlockState state)
{
    return false;
}

BlockFaceShape BlockAir::getBlockFaceShape(IBlockAccess worldIn, IBlockState state, BlockPos pos, EnumFacing face)
{
    return BlockFaceShape::UNDEFINED;
}
