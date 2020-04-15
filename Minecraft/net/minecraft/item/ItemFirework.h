#pragma once
#include "Item.h"

class ItemFirework :public Item
{
public:
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
};