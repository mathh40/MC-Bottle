#pragma once
#include "BlockContainer.h"

class BlockBanner : public BlockContainer {
public:
    static PropertyDirection FACING;
    static PropertyInteger ROTATION;

    std::string getLocalizedName() const override;
    std::optional<AxisAlignedBB>
    getCollisionBoundingBox(IBlockState blockState, IBlockAccess worldIn, BlockPos pos) override;
    bool isFullCube(IBlockState state) override;
    bool isPassable(IBlockAccess worldIn, BlockPos pos) override;
    bool isOpaqueCube(IBlockState state) override;
    bool canSpawnInBlock() override;
    std::optional<TileEntity> createNewTileEntity(World worldIn, int meta) override;
    Item getItemDropped(IBlockState state, pcg32 &rand, int32_t fortune) override;
    ItemStack getItem(World worldIn, BlockPos pos, IBlockState state) override;
    void dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState state, float chance,
                                   int32_t fortune) override;
    bool canPlaceBlockAt(World worldIn, BlockPos pos) override;
    void harvestBlock(World worldIn, EntityPlayer player, BlockPos pos, IBlockState state, std::optional<TileEntity> te,
                      ItemStack stack) override;
    BlockFaceShape getBlockFaceShape(IBlockAccess worldIn, IBlockState state, BlockPos pos, EnumFacing face) override;


protected:
    static AxisAlignedBB STANDING_AABB;

    BlockBanner();
private:
    ItemStack getTileDataItemStack(World worldIn, BlockPos pos);
};

class BlockBannerStanding : public BlockBanner {
public:
    BlockBannerStanding();
    AxisAlignedBB &getBoundingBox(IBlockState state, IBlockAccess source, BlockPos pos) override;
    IBlockState &withRotation(IBlockState state, Rotation rot) override;
    IBlockState &withMirror(IBlockState state, Mirror mirrorIn) override;
    void neighborChanged(IBlockState state, World worldIn, BlockPos pos, Block blockIn, BlockPos fromPos) override;
    IBlockState &getStateFromMeta(uint16_t meta) override;
    uint16_t getMetaFromState(IBlockState state) override;
protected:
    BlockStateContainer createBlockState() override;
};

class BlockBannerHanging : public BlockBanner {
public:
    BlockBannerHanging();
    IBlockState &withRotation(IBlockState state, Rotation rot) override;
    IBlockState &withMirror(IBlockState state, Mirror mirrorIn) override;
    AxisAlignedBB &getBoundingBox(IBlockState state, IBlockAccess source, BlockPos pos) override;
    void neighborChanged(IBlockState state, World worldIn, BlockPos pos, Block blockIn, BlockPos fromPos) override;
    IBlockState &getStateFromMeta(uint16_t meta) override;
    uint16_t getMetaFromState(IBlockState state) override;
protected:
    static AxisAlignedBB NORTH_AABB;
    static AxisAlignedBB SOUTH_AABB;
    static AxisAlignedBB WEST_AABB;
    static AxisAlignedBB EAST_AABB;

    BlockStateContainer createBlockState() override;
};
