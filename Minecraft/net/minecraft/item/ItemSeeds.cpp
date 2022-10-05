#include "ItemSeeds.h"

#include "ItemStack.h"
#include "World.h"

ItemSeeds::ItemSeeds(Block *crops, Block *soil) : crops(crops), soilBlockID(soil)
{
    setCreativeTab(CreativeTabs::MATERIALS);
}

EnumActionResult ItemSeeds::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                      EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (facing == EnumFacing::UP && player->canPlayerEdit(pos.offset(facing), facing, itemstack) &&
        worldIn->getBlockState(pos)->getBlock() == soilBlockID && worldIn->isAirBlock(pos.up()))
    {
        worldIn->setBlockState(pos.up(), crops->getDefaultState());
        if (Util:: instanceof <EntityPlayerMP>(player))
        {
            CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP *)player, pos.up(), itemstack);
        }

        itemstack.shrink(1);
        return EnumActionResult::SUCCESS;
    }
    else
    {
        return EnumActionResult::FAIL;
    }
}
