#include "BlockBanner.h"

#include "../util/EnumFacing.h"
#include "../util/Mirror.h"
#include "../util/Util.h"
#include "../util/text/translation/I18n.h"

PropertyDirection BlockBanner::FACING = BlockHorizontal.FACING;
PropertyInteger BlockBanner::ROTATION = PropertyInteger.create("rotation", 0, 15);
AxisAlignedBB BlockBanner::STANDING_AABB(0.25, 0.0, 0.25, 0.75, 1.0, 0.75);

AxisAlignedBB BlockBannerHanging::NORTH_AABB(0.0, 0.0, 0.875, 1.0, 0.78125, 1.0);
AxisAlignedBB BlockBannerHanging::SOUTH_AABB(0.0, 0.0, 0.0, 1.0, 0.78125, 0.125);
AxisAlignedBB BlockBannerHanging::WEST_AABB(0.875, 0.0, 0.0, 1.0, 0.78125, 1.0);
AxisAlignedBB BlockBannerHanging::EAST_AABB(0.0, 0.0, 0.0, 0.125, 0.78125, 1.0);

std::string BlockBanner::getLocalizedName() const
{
    return I18n::translateToLocal("item.banner.white.name");
}

BlockBanner::BlockBanner() : BlockContainer(Material.WOOD)
{
}

std::optional<AxisAlignedBB> BlockBanner::getCollisionBoundingBox(IBlockState blockState, IBlockAccess worldIn,
                                                                  BlockPos pos)
{
    return std::nullopt;
}

bool BlockBanner::isFullCube(IBlockState state)
{
    return false;
}

bool BlockBanner::isPassable(IBlockAccess worldIn, BlockPos pos)
{
    return true;
}

bool BlockBanner::isOpaqueCube(IBlockState state)
{
    return false;
}

bool BlockBanner::canSpawnInBlock()
{
    return true;
}

std::optional<TileEntity> BlockBanner::createNewTileEntity(World worldIn, int meta)
{
    return TileEntityBanner();
}

Item BlockBanner::getItemDropped(IBlockState state, pcg32 &rand, int32_t fortune)
{
    return Items.BANNER;
}

ItemStack BlockBanner::getTileDataItemStack(World worldIn, BlockPos pos)
{
    auto tileentity = worldIn.getTileEntity(pos);
    return Util:: instanceof
        <TileEntityBanner>(tileentity) ? ((TileEntityBanner)tileentity).getItem() : ItemStack.EMPTY;
}

BlockBannerStanding::BlockBannerStanding()
{
    setDefaultState(blockState.getBaseState().withProperty(ROTATION, 0));
}

AxisAlignedBB &BlockBannerStanding::getBoundingBox(IBlockState state, IBlockAccess source, BlockPos pos)
{
    return STANDING_AABB;
}

BlockBannerHanging::BlockBannerHanging()
{
    setDefaultState(blockState.getBaseState().withProperty(FACING, EnumFacing::NORTH));
}

ItemStack BlockBanner::getItem(World worldIn, BlockPos pos, IBlockState state)
{
    auto itemstack = getTileDataItemStack(worldIn, pos);
    return itemstack.isEmpty() ? ItemStack(Items.BANNER) : itemstack;
}

void BlockBanner::dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState state, float chance,
                                            int32_t fortune)
{
    auto itemstack = getTileDataItemStack(worldIn, pos);
    if (itemstack.isEmpty())
    {
        BlockContainer::dropBlockAsItemWithChance(worldIn, pos, state, chance, fortune);
    }
    else
    {
        spawnAsEntity(worldIn, pos, itemstack);
    }
}

bool BlockBanner::canPlaceBlockAt(World worldIn, BlockPos pos)
{
    return !hasInvalidNeighbor(worldIn, pos) && BlockContainer::canPlaceBlockAt(worldIn, pos);
}

void BlockBanner::harvestBlock(World worldIn, EntityPlayer player, BlockPos pos, IBlockState state,
                               std::optional<TileEntity> te, ItemStack stack)
{
    if (Util:: instanceof <TileEntityBanner>(te))
    {
        auto tileentitybanner = (TileEntityBanner)te;
        auto itemstack        = tileentitybanner.getItem();
        spawnAsEntity(worldIn, pos, itemstack);
    }
    else
    {
        BlockContainer::harvestBlock(worldIn, player, pos, state, std::nullopt, stack);
    }
}

BlockFaceShape BlockBanner::getBlockFaceShape(IBlockAccess worldIn, IBlockState state, BlockPos pos, EnumFacing face)
{
    return BlockFaceShape.UNDEFINED;
}

IBlockState &BlockBanner::withRotation(IBlockState state, Rotation rot)
{
    return state.withProperty(ROTATION, rot.rotate(state.getValue(ROTATION), 16));
}

IBlockState &BlockBanner::withMirror(IBlockState state, Mirror mirrorIn)
{
    return state.withProperty(ROTATION, mirrorIn.mirrorRotation(state.getValue(ROTATION), 16));
}

void BlockBanner::neighborChanged(IBlockState state, World worldIn, BlockPos pos, Block blockIn, BlockPos fromPos)
{
    if (!worldIn.getBlockState(pos.down()).getMaterial().isSolid())
    {
        this.dropBlockAsItem(worldIn, pos, state, 0);
        worldIn.setBlockToAir(pos);
    }

    BlockBanner::neighborChanged(state, worldIn, pos, blockIn, fromPos);
}

IBlockState &BlockBannerStanding::getStateFromMeta(uint16_t meta)
{
    return getDefaultState().withProperty(ROTATION, meta);
}

uint16_t BlockBannerStanding::getMetaFromState(IBlockState state)
{
    return state.getValue(BlockBanner::ROTATION);
}

BlockStateContainer BlockBannerStanding::createBlockState()
{
    return BlockStateContainer(this, IProperty[]{ROTATION});
}

IBlockState &BlockBannerHanging::withRotation(IBlockState state, Rotation rot)
{
    return state.withProperty(FACING, rot.rotate((EnumFacing)state.getValue(FACING)));
}

IBlockState &BlockBannerHanging::withMirror(IBlockState state, Mirror mirrorIn)
{
    return state.withRotation(mirrorIn.toRotation((EnumFacing)state.getValue(FACING)));
}

AxisAlignedBB &BlockBannerHanging::getBoundingBox(IBlockState state, IBlockAccess source, BlockPos pos)
{
    auto state = (EnumFacing)state.getValue(FACING);
    if (state == EnumFacing::NORTH)
        return NORTH_AABB;
    if (state == EnumFacing::SOUTH)
        return SOUTH_AABB;
    if (state == EnumFacing::WEST)
        return WEST_AABB;
    if (state == EnumFacing::EAST)
        return EAST_AABB;
}

void BlockBannerHanging::neighborChanged(IBlockState state, World worldIn, BlockPos pos, Block blockIn,
                                         BlockPos fromPos)
{
    auto enumfacing = (EnumFacing)state.getValue(FACING);
    if (!worldIn.getBlockState(pos.offset(enumfacing.getOpposite())).getMaterial().isSolid())
    {
        dropBlockAsItem(worldIn, pos, state, 0);
        worldIn.setBlockToAir(pos);
    }

    BlockBanner::neighborChanged(state, worldIn, pos, blockIn, fromPos);
}

IBlockState &BlockBannerHanging::getStateFromMeta(uint16_t meta)
{
    auto enumfacing = EnumFacing::byIndex(meta);
    if (enumfacing.getAxis() == Axis::Y)
    {
        enumfacing = EnumFacing::NORTH;
    }

    return getDefaultState().withProperty(FACING, enumfacing);
}

uint16_t BlockBannerHanging::getMetaFromState(IBlockState state)
{
    return ((EnumFacing)state.getValue(FACING)).getIndex();
}

BlockStateContainer BlockBannerHanging::createBlockState()
{
    return BlockStateContainer(this, IProperty[]{FACING});
}
