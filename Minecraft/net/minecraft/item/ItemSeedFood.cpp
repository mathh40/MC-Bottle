#include "ItemSeedFood.h"

#include "ItemStack.h"
#include "World.h"

ItemSeedFood::ItemSeedFood(int32_t healAmount, float saturation, Block *crops, Block *soil)
    :ItemFood(healAmount, saturation, false),crops(crops),soilId(soil)
{
}

EnumActionResult ItemSeedFood::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (facing == EnumFacing::UP && player->canPlayerEdit(pos.offset(facing), facing, itemstack) && worldIn->getBlockState(pos)->getBlock() == soilId && worldIn->isAirBlock(pos.up())) 
    {
        worldIn->setBlockState(pos.up(), crops->getDefaultState(), 11);
        itemstack.shrink(1);
        return EnumActionResult::SUCCESS;
    }
    else 
    {
        return EnumActionResult::FAIL;
    }
}
