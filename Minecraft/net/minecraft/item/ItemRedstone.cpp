#include "ItemRedstone.h"

#include "ItemStack.h"
#include "World.h"

ItemRedstone::ItemRedstone()
{
    setCreativeTab(CreativeTabs::REDSTONE);
}

EnumActionResult ItemRedstone::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    bool flag = worldIn->getBlockState(pos)->getBlock()->isReplaceable(worldIn, pos);
    BlockPos blockpos = flag ? pos : pos.offset(facing);
    ItemStack itemstack = player->getHeldItem(hand);
    if (player->canPlayerEdit(blockpos, facing, itemstack) && worldIn->mayPlace(worldIn->getBlockState(blockpos)->getBlock(), blockpos, false, facing, (Entity)null) && Blocks.REDSTONE_WIRE.canPlaceBlockAt(worldIn, blockpos)) 
    {
        worldIn->setBlockState(blockpos, Blocks.REDSTONE_WIRE.getDefaultState());
        if (Util::instanceof<EntityPlayerMP>(player)) 
        {
            CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP*)player, blockpos, itemstack);
        }

        itemstack.shrink(1);
        return EnumActionResult::SUCCESS;
    }
    else 
    {
        return EnumActionResult::FAIL;
    }
}
