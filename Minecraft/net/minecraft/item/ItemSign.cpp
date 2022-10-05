#include "ItemSign.h"

#include "../tileentity/TileEntitySign.h"
#include "ItemBlock.h"
#include "ItemStack.h"

ItemSign::ItemSign()
{
    maxStackSize = 16;
    setCreativeTab(CreativeTabs::DECORATIONS);
}

EnumActionResult ItemSign::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                     EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState *iblockstate = worldIn->getBlockState(pos);
    boolean flag             = iblockstate.getBlock().isReplaceable(worldIn, pos);
    if (facing != EnumFacing::DOWN && (iblockstate.getMaterial().isSolid() || flag) &&
        (!flag || facing == EnumFacing.UP))
    {
        pos                 = pos.offset(facing);
        ItemStack itemstack = player->getHeldItem(hand);
        if (player->canPlayerEdit(pos, facing, itemstack) && Blocks::STANDING_SIGN.canPlaceBlockAt(worldIn, pos))
        {
            if (worldIn->isRemote)
            {
                return EnumActionResult::SUCCESS;
            }
            else
            {
                pos = flag ? pos.down() : pos;
                if (facing == EnumFacing::UP)
                {
                    int i = MathHelper::floor((double)((player->rotationYaw + 180.0F) * 16.0F / 360.0F) + 0.5D) & 15;
                    worldIn->setBlockState(
                        pos, Blocks::STANDING_SIGN.getDefaultState().withProperty(BlockStandingSign::ROTATION, i), 11);
                }
                else
                {
                    worldIn->setBlockState(
                        pos, Blocks::WALL_SIGN.getDefaultState().withProperty(BlockWallSign::FACING, facing), 11);
                }

                TileEntity *tileentity = worldIn->getTileEntity(pos);
                if (Util:: instanceof <TileEntitySign>(tileentity) &&
                                          !ItemBlock::setTileEntityNBT(worldIn, player, pos, itemstack))
                {
                    player->openEditSign((TileEntitySign *)tileentity);
                }

                if (Util:: instanceof <EntityPlayerMP>(player))
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP *)player, pos, itemstack);
                }

                itemstack.shrink(1);
                return EnumActionResult::SUCCESS;
            }
        }
        else
        {
            return EnumActionResult::FAIL;
        }
    }
    else
    {
        return EnumActionResult::FAIL;
    }
}
