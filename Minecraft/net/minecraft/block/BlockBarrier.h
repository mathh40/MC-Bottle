#pragma once
#include "Block.h"

class BlockBarrier : public Block {
public:
    EnumBlockRenderType getRenderType(IBlockState state) override;
    bool isOpaqueCube(IBlockState state) override;
    float getAmbientOcclusionLightValue(IBlockState state) override;
    void dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState state, float chance,
                                   int32_t fortune) override;
protected:
    BlockBarrier();
    ~BlockBarrier() = default;
private:
};
