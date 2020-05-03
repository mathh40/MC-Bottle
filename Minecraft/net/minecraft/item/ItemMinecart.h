#pragma once
#include "Item.h"

class IBlockSource;

class ItemMinecart :public Item
{
public:
    ItemMinecart(EntityMinecart::Type typeIn);
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
private:
    EntityMinecart::Type minecartType;
};
