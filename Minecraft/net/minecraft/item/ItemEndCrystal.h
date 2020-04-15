#pragma once
#include "Item.h"

class ItemEndCrystal :public Item
{
public:
    ItemEndCrystal();
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    bool hasEffect(ItemStack stack) override;
};