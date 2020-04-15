#include "ItemBlockSpecial.h"

#include "EnumFacing.h"
#include "ItemBlock.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "Util.h"
#include "../block/SoundType.h"

ItemBlockSpecial::ItemBlockSpecial(Block *block)
    :block(block)
{
}

EnumActionResult ItemBlockSpecial::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState* iblockstate = worldIn->getBlockState(pos);
    Block* block = iblockstate->getBlock();
    if (block == Blocks::SNOW_LAYER && iblockstate->getValue(BlockSnow::LAYERS) < 1) 
    {
        facing = EnumFacing::UP;
    }
    else if (!block->isReplaceable(worldIn, pos)) 
    {
        pos = pos.offset(facing);
    }

    ItemStack itemstack = player->getHeldItem(hand);
    if (!itemstack.isEmpty() && player->canPlayerEdit(pos, facing, itemstack) && worldIn->mayPlace(block, pos, false, facing, nullptr)) 
    {
        IBlockState* iblockstate1 = block->getStateForPlacement(worldIn, pos, facing, hitX, hitY, hitZ, 0, player);
        if (!worldIn->setBlockState(pos, iblockstate1, 11)) 
        {
            return EnumActionResult::FAIL;
        }
        else 
        {
            iblockstate1 = worldIn->getBlockState(pos);
            if (iblockstate1->getBlock() == block) 
            {
                ItemBlock::setTileEntityNBT(worldIn, player, pos, itemstack);
                iblockstate1->getBlock().onBlockPlacedBy(worldIn, pos, iblockstate1, player, itemstack);
                if (Util::instanceof<EntityPlayerMP>(player)) 
                {
                    CriteriaTriggers::PLACED_BLOCK::trigger((EntityPlayerMP*)player, pos, itemstack);
                }
            }

            SoundType soundtype = block->getSoundType();
            worldIn->playSound(player, pos, soundtype.getPlaceSound(), SoundCategory::BLOCKS, (soundtype.getVolume() + 1.0F) / 2.0F, soundtype.getPitch() * 0.8F);
            itemstack.shrink(1);
            return EnumActionResult::SUCCESS;
        }
    }
    else 
    {
        return EnumActionResult::FAIL;
    }
}
