#include "BlockBarrier.h"

BlockBarrier::BlockBarrier() :
    Block(Material.BARRIER) {
    setBlockUnbreakable();
    setResistance(6000001.0F);
    disableStats();
    translucent = true;
}

EnumBlockRenderType BlockBarrier::getRenderType(IBlockState state) {
    return EnumBlockRenderType::INVISIBLE;
}

bool BlockBarrier::isOpaqueCube(IBlockState state) {
    return false;
}

float BlockBarrier::getAmbientOcclusionLightValue(IBlockState state) {
    return 1.0F;
}

void BlockBarrier::dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState state, float chance,
                                             int32_t fortune) {

}
