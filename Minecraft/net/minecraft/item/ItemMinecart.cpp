#include "ItemMinecart.h"

#include "ItemStack.h"
#include "../world/World.h"

ItemMinecart::ItemMinecart(EntityMinecart::Type typeIn)
{
    maxStackSize = 1;
    minecartType = typeIn;
    setCreativeTab(CreativeTabs::TRANSPORTATION);
    BlockDispenser::DISPENSE_BEHAVIOR_REGISTRY.putObject(this, MINECART_DISPENSER_BEHAVIOR);
}

EnumActionResult ItemMinecart::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    IBlockState* iblockstate = worldIn->getBlockState(pos);
    if (!BlockRailBase.isRailBlock(iblockstate)) 
    {
        return EnumActionResult::FAIL;
    }
    else 
    {
        ItemStack itemstack = player->getHeldItem(hand);
        if (!worldIn->isRemote) 
        {
            BlockRailBase::EnumRailDirection blockrailbase$enumraildirection = iblockstate->getBlock() instanceof BlockRailBase ? (BlockRailBase.EnumRailDirection)iblockstate->getValue(((BlockRailBase)iblockstate->getBlock()).getShapeProperty()) : BlockRailBase.EnumRailDirection.NORTH_SOUTH;
            double d0 = 0.0D;
            if (blockrailbase$enumraildirection.isAscending()) 
            {
                d0 = 0.5D;
            }

            EntityMinecart entityminecart = EntityMinecart.create(worldIn, (double)pos.getx() + 0.5, (double)pos.gety() + 0.0625 + d0, (double)pos.getz() + 0.5, minecartType);
            if (itemstack.hasDisplayName()) 
            {
                entityminecart.setCustomNameTag(itemstack.getDisplayName());
            }

            worldIn->spawnEntity(entityminecart);
        }

        itemstack.shrink(1);
        return EnumActionResult::SUCCESS;
    }
}
