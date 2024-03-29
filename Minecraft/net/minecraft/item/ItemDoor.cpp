#include "ItemDoor.h"

#include "EnumFacing.h"
#include "ItemStack.h"
#include "SoundCategory.h"

ItemDoor::ItemDoor(Block *block) : block(block)
{
    setCreativeTab(CreativeTabs::REDSTONE);
}

EnumActionResult ItemDoor::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                     EnumFacing facing, float hitX, float hitY, float hitZ)
{
    if (facing != EnumFacing::UP)
    {
        return EnumActionResult::FAIL;
    }
    else
    {
        IBlockState *iblockstate = worldIn->getBlockState(pos);
        Block *block             = iblockstate->getBlock();
        if (!block->isReplaceable(worldIn, pos))
        {
            pos = pos.offset(facing);
        }

        ItemStack itemstack = player->getHeldItem(hand);
        if (player->canPlayerEdit(pos, facing, itemstack) && block->canPlaceBlockAt(worldIn, pos))
        {
            EnumFacing enumfacing = EnumFacing::fromAngle((double)player->rotationYaw);
            int32_t i             = enumfacing.getXOffset();
            int32_t j             = enumfacing.getZOffset();
            bool flag = i < 0 && hitZ < 0.5F || i > 0 && hitZ > 0.5F || j < 0 && hitX > 0.5F || j > 0 && hitX < 0.5F;
            placeDoor(worldIn, pos, enumfacing, block, flag);
            SoundType soundtype = block->getSoundType();
            worldIn->playSound(player, pos, soundtype.getPlaceSound(), SoundCategory::BLOCKS,
                               (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
            itemstack.shrink(1);
            return EnumActionResult::SUCCESS;
        }
        else
        {
            return EnumActionResult::FAIL;
        }
    }
}

void ItemDoor::placeDoor(World *worldIn, BlockPos pos, EnumFacing facing, Block *door, bool isRightHinge)
{
    BlockPos blockpos  = pos.offset(facing.rotateY());
    BlockPos blockpos1 = pos.offset(facing.rotateYCCW());
    int i              = (worldIn->getBlockState(blockpos1).isNormalCube() ? 1 : 0) +
            (worldIn->getBlockState(blockpos1.up()).isNormalCube() ? 1 : 0);
    int j = (worldIn->getBlockState(blockpos).isNormalCube() ? 1 : 0) +
            (worldIn->getBlockState(blockpos.up()).isNormalCube() ? 1 : 0);
    bool flag = worldIn->getBlockState(blockpos1).getBlock() == door ||
                worldIn->getBlockState(blockpos1.up()).getBlock() == door;
    bool flag1 =
        worldIn->getBlockState(blockpos).getBlock() == door || worldIn->getBlockState(blockpos.up()).getBlock() == door;
    if ((!flag || flag1) && j <= i)
    {
        if (flag1 && !flag || j < i)
        {
            isRightHinge = false;
        }
    }
    else
    {
        isRightHinge = true;
    }

    BlockPos blockpos2       = pos.up();
    bool flag2               = worldIn->isBlockPowered(pos) || worldIn->isBlockPowered(blockpos2);
    IBlockState *iblockstate = door->getDefaultState()
                                   ->withProperty(BlockDoor::FACING, facing)
                                   .withProperty(BlockDoor::HINGE, isRightHinge ? BlockDoor::EnumHingePosition.RIGHT
                                                                                : BlockDoor::EnumHingePosition.LEFT)
                                   .withProperty(BlockDoor::POWERED, flag2)
                                   .withProperty(BlockDoor::OPEN, flag2);
    worldIn->setBlockState(pos, iblockstate->withProperty(BlockDoor::HALF, BlockDoor::EnumDoorHalf::LOWER), 2);
    worldIn->setBlockState(blockpos2, iblockstate->withProperty(BlockDoor::HALF, BlockDoor::EnumDoorHalf::UPPER), 2);
    worldIn->notifyNeighborsOfStateChange(pos, door, false);
    worldIn->notifyNeighborsOfStateChange(blockpos2, door, false);
}
