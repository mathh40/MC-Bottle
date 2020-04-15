#pragma once
#include "Item.h"

class ItemBlockSpecial :public Item
{
public:
    ItemBlockSpecial(Block* block);
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
private:
    Block* block;
};