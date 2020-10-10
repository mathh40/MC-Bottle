#pragma once
#include "Block.h"
#include "ITileEntityProvider.h"

class BlockContainer : public Block, ITileEntityProvider {
public:
    EnumBlockRenderType getRenderType(IBlockState state) override;
    void breakBlock(World worldIn, BlockPos pos, IBlockState state) override;
    void harvestBlock(World worldIn, EntityPlayer player, BlockPos pos, IBlockState state, std::optional<TileEntity> te,
                      ItemStack stack) override;
    bool eventReceived(IBlockState state, World worldIn, BlockPos pos, int32_t id, int32_t param) override;
protected:
    ~BlockContainer() = default;
    BlockContainer(Material materialIn);
    BlockContainer(Material materialIn, MapColor color);
    bool isInvalidNeighbor(World worldIn, BlockPos pos, EnumFacing facing);
    bool hasInvalidNeighbor(World worldIn, BlockPos pos);
private:
};
